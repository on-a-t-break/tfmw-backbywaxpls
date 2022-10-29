/*
    Remove a template from the supported templates.

    @param {eosio::name} owner - address claiming
    @param {uint64_t} asset_id - asset to be backed

    @throw Will throw if the contract is in maintenance
    @throw Will throw if the user does not own the asset
    @throw Will throw if template is not supported
    @throw Will throw if the asset is already backed (at max value)
    @throw Will throw if there is no balance left to back

    @auth self
*/
[[eosio::action]] void tokenreward::claimtokens(
    eosio::name & owner, 
    uint64_t asset_id
)
{
    // Check auth
    require_auth(owner);
    
    // Internal asset backing helper
    auto result = backasset(owner, asset_id, true);

    // Error handling
    switch (result) {
        case MAINTENANCE:
            eosio::check(false, "Contract is in maintenance");
            break;

        case ASSET_NOT_OWNED: 
            eosio::check(false, "Asset not owned");
            break;

        case TEMPLATE_NOT_SUPPORTED:
            eosio::check(false, "Template not supported");
            break;

        case ASSET_ALREADY_BACKED:
            eosio::check(false, "Asset already at max backed balance");
            break;
    };

}