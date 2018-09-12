//
//  blocksci.cpp
//  BlockReader
//
//  Created by Harry Kalodner on 1/14/17.
//  Copyright © 2017 Harry Kalodner. All rights reserved.
//

#include "caster_py.hpp"
#include "blocksci_range.hpp"

#include "chain/blockchain_py.hpp"
#include "chain/input/input_py.hpp"
#include "chain/input/input_range_py.hpp"
#include "chain/output/output_py.hpp"
#include "chain/output/output_range_py.hpp"
#include "chain/tx/tx_py.hpp"
#include "chain/tx/tx_range_py.hpp"
#include "chain/block/block_py.hpp"
#include "chain/block/block_range_py.hpp"

#include "cluster/cluster/cluster_py.hpp"
#include "cluster/cluster/cluster_range_py.hpp"
#include "cluster/tagged_cluster/tagged_cluster_py.hpp"
#include "cluster/tagged_cluster/tagged_cluster_range_py.hpp"
#include "cluster/tagged_address/tagged_address_py.hpp"
#include "cluster/tagged_address/tagged_address_range_py.hpp"

#include "scripts/address_py.hpp"
#include "scripts/address_range_py.hpp"
#include "scripts/equiv_address/equiv_address_py.hpp"
#include "scripts/equiv_address/equiv_address_range_py.hpp"
#include "scripts/pubkey/pubkey_py.hpp"
#include "scripts/pubkey/pubkey_range_py.hpp"
#include "scripts/pubkey/pubkeyhash_range_py.hpp"
#include "scripts/pubkey/witness_pubkeyhash_range_py.hpp"
#include "scripts/pubkey/multisig_pubkey_range_py.hpp"
#include "scripts/multisig/multisig_py.hpp"
#include "scripts/multisig/multisig_range_py.hpp"
#include "scripts/scripthash/scripthash_py.hpp"
#include "scripts/scripthash/scripthash_range_py.hpp"
#include "scripts/scripthash/witness_scripthash_range_py.hpp"
#include "scripts/nulldata/nulldata_py.hpp"
#include "scripts/nulldata/nulldata_range_py.hpp"
#include "scripts/nonstandard/nonstandard_py.hpp"
#include "scripts/nonstandard/nonstandard_range_py.hpp"

namespace py = pybind11;

using namespace blocksci;

void init_blockchain(py::module &m);
void init_ranges(py::module &m);
void init_heuristics(py::module &m);

PYBIND11_MODULE(_blocksci, m) {
    m.attr("__name__") = PYBIND11_STR_TYPE("blocksci");
    init_ranges(m);
    py::class_<Blockchain> blockchainCl(m, "Blockchain", "Class representing the blockchain. This class is contructed by passing it a string representing a file path to your BlockSci data files generated by blocksci_parser", py::dynamic_attr());
    py::class_<uint160> uint160Cl(m, "uint160");
    py::class_<uint256> uint256Cl(m, "uint256");
    py::class_<Block> blockCl(m, "Block", "Class representing a block in the blockchain");
    py::class_<Transaction> txCl(m, "Tx", "Class representing a transaction in a block");
    py::class_<Input> inputCl(m, "Input", "Class representing a transaction input");
    py::class_<Output> outputCl(m, "Output", "Class representing a transaction output");
    py::class_<EquivAddress> equivAddressCl(m, "EquivAddress", "A set of equivalent addresses");
    py::class_<ScriptBase> addressCl(m, "Address", "Represents an abstract address object which uniquely identifies a given address");
    py::class_<script::Pubkey> pubkeyAddressCl(m, "PubkeyAddress", addressCl, "Extra data about pay to pubkey address");
    py::class_<script::PubkeyHash> pubkeyHashAddressCl(m, "PubkeyHashAddress", addressCl, "Extra data about pay to pubkey address");
    py::class_<script::WitnessPubkeyHash> witnessPubkeyHashAddressCl(m, "WitnessPubkeyHashAddress", addressCl, "Extra data about pay to pubkey address");
    py::class_<script::MultisigPubkey> multisigPubkeyCl(m, "MultisigPubkey", addressCl, "Extra data about a pubkey inside a multisig address");
    py::class_<script::ScriptHash> scriptHashCl(m, "ScriptHashAddress", addressCl, "Extra data about pay to script hash address");
    py::class_<script::WitnessScriptHash> witnessScriptHashCl(m, "WitnessScriptHashAddress", addressCl, "Extra data about pay to script hash address");
    py::class_<script::Multisig> multisigCl(m, "MultisigAddress", addressCl, "Extra data about multi-signature address");
    py::class_<script::Nonstandard> nonstandardCl(m, "NonStandardAddress", addressCl, "Extra data about non-standard address");
    py::class_<script::OpReturn> opReturnCl(m, "OpReturn", addressCl, "Extra data about op_return address");

    auto clusterMod = m.def_submodule("cluster");
    py::class_<Cluster> clusterCl(clusterMod, "Cluster", "Class representing a cluster");
    py::class_<TaggedCluster> taggedClusterCl(clusterMod, "TaggedCluster");
    py::class_<TaggedAddress> taggedAddressCl(clusterMod, "TaggedAddress");

    BlockRangeClasses blockRangeCls(m);
    RangeClasses<Transaction> txRangeCls(m);
    RangeClasses<Input> inputRangeCls(m);
    RangeClasses<Output> outputRangeCls(m);
    RangeClasses<AnyScript> addressRangeCls(m);
    RangeClasses<EquivAddress> equivAddressRangeCls(m);
    RangeClasses<script::Pubkey> pubkeyRangeCls(m);
    RangeClasses<script::PubkeyHash> pubkeyHashRangeCls(m);
    RangeClasses<script::WitnessPubkeyHash> witnessPubkeyHashRangeCls(m);
    RangeClasses<script::MultisigPubkey> multisigSubkeyRangeCls(m);
    RangeClasses<script::Multisig> multisigRangeCls(m);
    RangeClasses<script::ScriptHash> scripthashRangeCls(m);
    RangeClasses<script::WitnessScriptHash> witnessScripthashRangeCls(m);
    RangeClasses<script::OpReturn> nulldataRangeCls(m);
    RangeClasses<script::Nonstandard> nonstandardRangeCls(m);
    RangeClasses<Cluster> clusterRangeCls(m);
    RangeClasses<TaggedCluster> taggedClusterRangeCls(m);
    RangeClasses<TaggedAddress> taggedAddressRangeCls(m);

    init_address_type(m);
    init_heuristics(m);
    init_data_access(m);
    init_blockchain(blockchainCl);
    init_uint160(uint160Cl);
    init_uint256(uint256Cl);
    {
        init_equiv_address(equivAddressCl);
        addEquivAddressRangeMethods(equivAddressRangeCls);
        applyMethodsToEquivAddressRange(equivAddressRangeCls);
    }
    {
        init_block(blockCl);
        addBlockRangeMethods(blockRangeCls);
        applyMethodsToBlockRange(blockRangeCls);
    }
    {
        init_tx(txCl);
        addTxRangeMethods(txRangeCls);
        applyMethodsToTxRange(txRangeCls);
        applyRangeFiltersToTxRange(txRangeCls);
    }
    {
        init_input(inputCl);
        addInputRangeMethods(inputRangeCls);
        applyMethodsToInputRange(inputRangeCls);
        applyRangeFiltersToInputRange(inputRangeCls);
    }
    {
        init_output(outputCl);
        addOutputRangeMethods(outputRangeCls);
        applyMethodsToOutputRange(outputRangeCls);
        applyRangeFiltersToOutputRange(outputRangeCls);
    }
    {
        {   
            addAddressRangeMethods(addressRangeCls);
            applyMethodsToAddressRange(addressRangeCls);
            applyRangeFiltersToAddressRange(addressRangeCls);
        }
        {
            init_pubkey(pubkeyAddressCl);
            addPubkeyRangeMethods(pubkeyRangeCls);
            applyMethodsToPubkeyRange(pubkeyRangeCls);
        }
        {
            init_pubkeyhash(pubkeyHashAddressCl);
            addPubkeyHashRangeMethods(pubkeyHashRangeCls);
            applyMethodsToPubkeyHashRange(pubkeyHashRangeCls);
        }
        {
            init_witness_pubkeyhash(witnessPubkeyHashAddressCl);
            addWitnessPubkeyHashRangeMethods(witnessPubkeyHashRangeCls);
            applyMethodsToWitnessPubkeyHashRange(witnessPubkeyHashRangeCls);
        }
        {
            init_multisig_pubkey(multisigPubkeyCl);
            addMultisigPubkeyRangeMethods(multisigSubkeyRangeCls);
            applyMethodsToMultisigPubkeyRange(multisigSubkeyRangeCls);
        }
        {
            init_multisig(multisigCl);
            addMultisigRangeMethods(multisigRangeCls);
            applyMethodsToMultisigRange(multisigRangeCls);
        }
        {
            init_scripthash(scriptHashCl);
            addScriptHashRangeMethods(scripthashRangeCls);
            applyMethodsToScriptHashRange(scripthashRangeCls);
        }
        {
            init_witness_scripthash(witnessScriptHashCl);
            addWitnessScriptHashRangeMethods(witnessScripthashRangeCls);
            applyMethodsToWitnessScriptHashRange(witnessScripthashRangeCls);
        }
        {
            init_nulldata(opReturnCl);
            addNulldataRangeMethods(nulldataRangeCls);
            applyMethodsToNulldataRange(nulldataRangeCls);
        }
        {
            init_nonstandard(nonstandardCl);
            addNonstandardRangeMethods(nonstandardRangeCls);
            applyMethodsToNonstandardRange(nonstandardRangeCls);
        }
    }
    {
        init_cluster_manager(clusterMod);
        {
            init_cluster(clusterCl);
            addClusterRangeMethods(clusterRangeCls);
            applyMethodsToClusterRange(clusterRangeCls);
        }
        {
            init_tagged_cluster(taggedClusterCl);
            addTaggedClusterRangeMethods(taggedClusterRangeCls);
            applyMethodsToTaggedClusterRange(taggedClusterRangeCls);
        }
        {
            init_tagged_address(taggedAddressCl);
            addTaggedAddressRangeMethods(taggedAddressRangeCls);
            applyMethodsToTaggedAddressRange(taggedAddressRangeCls);
        }
    }
    init_address(addressCl);
}