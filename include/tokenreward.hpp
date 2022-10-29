#pragma once

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/singleton.hpp>

#include <atomicassets.hpp>

CONTRACT tokenreward : public eosio::contract
{
public:
  using eosio::contract::contract;

  [[eosio::action]] void claimtokens(
      eosio::name & owner,
      uint64_t asset_id);

  [[eosio::action]] void settemplate(
      int32_t template_id,
      eosio::asset & token);

  [[eosio::action]] void rmtemplate(
      int32_t template_id);

  [[eosio::action]] void maintenance(
      bool maintenance);

  [[eosio::action]] void init();
  [[eosio::action]] void destruct();

  [[eosio::on_notify("atomicassets::logmint")]] void logmint(
      uint64_t asset_id,
      eosio::name authorized_minter,
      eosio::name collection_name,
      eosio::name schema_name,
      int32_t template_id,
      eosio::name new_asset_owner,
      atomicassets::ATTRIBUTE_MAP immutable_data,
      atomicassets::ATTRIBUTE_MAP mutable_data,
      std::vector<eosio::asset> backed_tokens,
      atomicassets::ATTRIBUTE_MAP immutable_template_data);

private:
  struct [[eosio::table("config")]] config_entity
  {
    bool maintenance = true;
  };

  typedef eosio::singleton<eosio::name("config"), config_entity> _config;

  struct [[eosio::table]] template_entity
  {
    int32_t template_id;
    eosio::asset token;

    uint64_t primary_key() const { return (uint64_t)template_id; }
  };

  typedef eosio::multi_index<eosio::name("templates"), template_entity> _templates;

  _config get_config()
  {
    return _config(get_self(), get_self().value);
  }

  _templates get_templates()
  {
    return _templates(get_self(), get_self().value);
  }

  enum backasset_result
  {
    OK,
    MAINTENANCE,
    ASSET_NOT_OWNED,
    TEMPLATE_NOT_SUPPORTED,
    ASSET_ALREADY_BACKED
  };

  backasset_result backasset(
      eosio::name & owner,
      uint64_t asset_id,
      bool ownership_check);
};
