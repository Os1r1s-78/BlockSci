//
//  parser_index.hpp
//  blocksci
//
//  Created by Harry Kalodner on 9/13/17.
//
//

#ifndef parser_index_hpp
#define parser_index_hpp

#include "parser_configuration.hpp"
#include "block_processor.hpp"
#include "progress_bar.hpp"

#include <blocksci/blocksci_fwd.hpp>
#include <blocksci/util/state.hpp>

#include <blocksci/address/equiv_address.hpp>
#include <blocksci/chain/chain_access.hpp>
#include <blocksci/chain/transaction_range.hpp>
#include <blocksci/scripts/script_access.hpp>

#include <range/v3/range_for.hpp>

#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>

#include <iostream>
#include <future>

namespace blocksci {
    struct Address;
}

template <typename T>
struct ParserIndexInfo;

template <typename T>
class ParserIndex {
protected:
    const ParserConfigurationBase &config;
    boost::filesystem::path cachePath;
    blocksci::State latestState;
public:
    ParserIndex(const ParserConfigurationBase &config_, const std::string &resultName) : config(config_), cachePath(config_.parserDirectory()/(resultName + ".txt")) {
        if (boost::filesystem::exists(cachePath)) {
            boost::filesystem::ifstream inputFile(cachePath);
            inputFile >> latestState;
        }
    }
    ParserIndex(const ParserIndex &) = delete;
    ParserIndex &operator=(const ParserIndex &) = delete;
    ParserIndex(ParserIndex &&) = delete;
    ParserIndex &operator=(ParserIndex &&) = delete;
    ~ParserIndex() {
        boost::filesystem::ofstream outputFile(cachePath);
        outputFile << latestState;
    }
    
    template<blocksci::EquivAddressType::Enum type, typename std::enable_if_t<ParserIndexInfo<T>::processesScript(type), int> = 0>
    void updateScript(const blocksci::State &state, const blocksci::ChainAccess &chain, const blocksci::ScriptAccess &scripts) {
        auto typeIndex = static_cast<size_t>(type);
        auto progress = makeProgressBar(state.scriptCounts[typeIndex] - latestState.scriptCounts[typeIndex], [=]() {});
        uint32_t num = 0;
        std::cout << "Updating index with scripts of type " << equivAddressName(type) << "\n";
        for (uint32_t i = latestState.scriptCounts[typeIndex]; i < state.scriptCounts[typeIndex]; i++) {
            static_cast<T*>(this)->template processScript<type>(i + 1, chain, scripts);
            progress.update(num);
            num++;
        }
    }
    
    template<blocksci::EquivAddressType::Enum type, typename std::enable_if_t<!ParserIndexInfo<T>::processesScript(type), int> = 0>
    void updateScript(const blocksci::State &, const blocksci::ChainAccess &, const blocksci::ScriptAccess &) {}
    
    virtual void prepareUpdate() {}
    void runUpdate(const blocksci::State &state) {
        blocksci::ChainAccess chain{config, false, blocksci::BlockHeight{0}};
        blocksci::ScriptAccess scripts{config};
        
        if (latestState.txCount < state.txCount) {
            auto newTransactions = blocksci::TransactionRange(chain, latestState.txCount, state.txCount);
            auto newCount = ranges::distance(newTransactions);
            std::cout << "Updating index with " << newCount << " txes\n";
            auto progress = makeProgressBar(newCount, [=]() {});
            uint32_t num = 0;
            RANGES_FOR(auto tx, newTransactions) {
                static_cast<T*>(this)->processTx(tx, scripts);
                progress.update(num);
                num++;
            }
        }
        
        blocksci::for_each(blocksci::EquivAddressInfoList(), [&](auto type) {
            updateScript<type>(state, chain, scripts);
        });
        latestState = state;
    };
    
    virtual void tearDown() {}
};

#endif /* parser_index_hpp */
