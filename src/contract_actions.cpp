/*
    Adds a template to the contract's "templates" table to mark it as a supported template.

    If this template is added it means that the contract will try to back the newly minted asset with the described token.

    Only tokens supported by AtomicAssets are supported.

    @param {int32_t} template_id - the atomicassets template id
    @param {eosio::asset} token - the token to back the asset with, use full precisions eg for wax: 19.00000000 WAX

    @throw Will throw if the token quantity is not bigger than 0

    @auth self
*/
[[eosio::action]] void tokenreward::settemplate(
    int32_t template_id,
    eosio::asset &token)
{
    // Check auth
    require_auth(get_self());

    // Check amount
    eosio::check(token.amount > 0, "Quantity must be positive");

    // Find item
    auto templates = get_templates();
    auto template_itr = templates.find(template_id);

    if (template_itr == templates.end())
    {
        // Item not found, insert a new one
        templates.emplace(get_self(), [&](auto &row)
                          {
                              row.template_id = template_id;
                              row.token = token;
                          });
    }
    else
    {
        // Item found, modify token field only
        templates.modify(template_itr, get_self(), [&](auto &row)
                         { row.token = token; });
    }
}

/*
    Remove a template from the supported templates.

    @param {int32_t} template_id - the atomicassets template id

    @throw Will throw if the entry could not be found

    @auth self
*/
[[eosio::action]] void tokenreward::rmtemplate(
    int32_t template_id)
{
    // Check auth
    require_auth(get_self());

    // Find item
    auto templates = get_templates();
    auto template_itr = templates.require_find(template_id, "No target auction found");

    // Delete
    templates.erase(template_itr);
}

/*
    Set the contract in maintenance mode.

    Maintenance mode prevents users from doing any actions.

    @param {bool} maintenance - maintenance toggle

    @auth self
*/
[[eosio::action]] void tokenreward::maintenance(
    bool maintenance)
{
    require_auth(get_self());

    auto config = get_config();
    auto new_config = config.get();

    new_config.maintenance = maintenance;

    config.set(new_config, get_self());
}

/*
    Initialize the contract configuration singleton, removes the previous configuration first.

    @auth self
*/
[[eosio::action]] void tokenreward::init()
{
    require_auth(get_self());
    get_config().remove();

    get_config().set(config_entity{}, get_self());
}

/*
    Delete the contract configuration singleton.

    @auth self
*/
[[eosio::action]] void tokenreward::destruct()
{
    require_auth(get_self());
    get_config().remove();
}
