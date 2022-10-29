#include <tokenreward.hpp>

#include "contract_actions.cpp"
#include "user_actions.cpp"

/*
  Helper function, not an invokeable action

  Backs the asset with a certain token.
  If the asset is already backed it will only back the asset for the difference.

  @param {eosio::name} owner - owner of the asset/nft
  @param {uint64_t} asset_id - the id of the asset/nft
  @param {bool} ownership_check - flag to check the asset/nft ownership

  @returns {backasset_result} Status of the asset backing, useful for error handling in user invokeable actions
*/
tokenreward::backasset_result tokenreward::backasset(
    eosio::name &owner,
    uint64_t asset_id,
    bool ownership_check)
{
  // Contract in maitenance, do not do anything
  if (!get_config().exists() || get_config().get().maintenance)
  {
    return MAINTENANCE;
  }

  // Get owner's assets
  auto owner_atomicassets = atomicassets::get_assets(owner);

  // Check ownership when ownership_check is enabled
  if (ownership_check && owner_atomicassets.find(asset_id) == owner_atomicassets.end())
  {
    return ASSET_NOT_OWNED;
  }

  // Get the atomicasset object (requires get instead of find for the backed_tokens)
  auto atomicasset = owner_atomicassets.get(asset_id);

  auto templates = get_templates();
  auto template_itr = templates.find(atomicasset.template_id);

  // Check if the template is supported, if not silently quit
  if (template_itr == templates.end())
  {
    return TEMPLATE_NOT_SUPPORTED;
  }

  auto token_to_back = template_itr->token;

  // Check if the asset is already backed in this asset
  auto existing_backed_token = std::find_if(
      atomicasset.backed_tokens.begin(),
      atomicasset.backed_tokens.end(),
      [&token = token_to_back](const eosio::asset &backed_token) -> bool
      { return token.symbol == backed_token.symbol; });

  if (existing_backed_token != atomicasset.backed_tokens.end())
  {
    // Token already backed

    if (existing_backed_token->amount >= token_to_back.amount)
    {
      // Skip, the amount of the already backed token is more than what we want to back

      return ASSET_ALREADY_BACKED;
    }

    // Amount cannot underflow as token.amount > existing_backed_token.amount
    token_to_back.set_amount(token_to_back.amount - existing_backed_token->amount);
  }

  // Back asset
  // Args: payer, asset_owner, asset_id, token_to_back
  eosio::action(
      eosio::permission_level{get_self(), eosio::name("active")},
      atomicassets::ATOMICASSETS_ACCOUNT,
      eosio::name("backasset"),
      std::make_tuple(
          get_self(),
          owner,
          asset_id,
          token_to_back))
      .send();

  return OK;
}

/*
  Listens for logmint notifications

  Does nothing when the contract is in maintenance (it does NOT fail, this would else prevent asset minting)

  Backs asset when the template is supported

  @auth atomicassets
*/
void tokenreward::logmint(
    uint64_t asset_id,
    eosio::name authorized_minter,
    eosio::name collection_name,
    eosio::name schema_name,
    int32_t template_id,
    eosio::name new_asset_owner,
    atomicassets::ATTRIBUTE_MAP immutable_data,
    atomicassets::ATTRIBUTE_MAP mutable_data,
    std::vector<eosio::asset> backed_tokens,
    atomicassets::ATTRIBUTE_MAP immutable_template_data)
{
  // Require auth of atomicassets
  require_auth(atomicassets::ATOMICASSETS_ACCOUNT);

  // We are dealing with a non template asset, exit silently
  if (template_id == 0)
  {
    return;
  }

  // Internal asset backing helper
  backasset(new_asset_owner, asset_id, false);
}
