The backbywaxpls smart contract was commissioned by "on a t break" & developed by "TheLonely" for Terraformers, Many Worlds, A.K.A, TFMW, on the Wax Blockchain. 

The purpose of this smart contract is to allow the autonomous Wax backing of NFTs.

Backbywaxpls works by setting up individual templates that will trigger a notify action when they are minted to be backed the designated amount of WAXP tokens.


Requirements:
- EOS based blockchains smart contract compiler.
-> The wallet that you will deploy this smart contract under will need to be added as a Notify Account.
-> A reserve of WAXP tokens sent to the "atomicassets" contract from the wallet that has deployed this contract.
-> Around 400kb of RAM to upload the contract, as well as an additional 128 bytes of RAM per template that you wish to be backed
-> Around 25 WAXP CPU & 5 WAXP NET in staking


Limitations:
-> This contract handles the autonomous backing of NFTs, however, it does not handle the WAXP tokens that need to be deposited into the "atomicassets" contract for it to back assets with.
-> Failure to have the required WAXP tokens (as designated by the template on your deployed contract) will result in a failure to mint said asset, until either the template is removed from the contract list or WAXP tokens have been provided.


The following instructions have been provided for demonstrative purposes on the testnet.


Step 0: Compile contract. Project settings: root = src/tokenreward.cpp, name = tokenreward.

Step 1: Upload contract <https://wax-test.bloks.io/wallet/utilities/upload-contract>

Step 2: Add "eosio.code" permission under the active key of the wallet you are deploying this contract from: <https://wax-test.bloks.io/wallet/permissions/advanced>
-> You can verify if you have done it correctly by comparing it to our deployment here: <https://wax-test.bloks.io/account/backtokencon#keys>
-> If you plan to use another smart contract to deposit WAXP tokens into backbywaxpls, you need to also add said contract permissions under the active key, as you will be required to send the WAXP tokens on your deployed version of backbywaxpls to "atomicassets".


WARNING:
-> Practice caution when performing any actions that have to do with wallet permissions. You can very well brick a wallet by making a mistake here. 
-> Any actions that have to do with changing wallet permissions will trigger a "forbidden actions" warning from Anchor. 
-> You will have to temporarily allow them in your settings & then turn them back off. 
-> This step is only required to be done once.
-> You are NOT supposed to add our wallet, which is "backbywaxpls", under the active permissions. 
-> You are supposed to add the wallet that you are deploying the contract from. 
-> You are essentially giving your own wallet permission. We, nor anyone else, is receiving any permissions.
-> We are not responsible for any harm that arises from this, nor any other step. 


Step 3: Add the wallet that you have deployed backbywaxpls under as a Notify Account under your collection.

Step 4: On your deployed contract, perform the "init" transaction.

Step 5: Under the "settemplate" action, put in the template_id of the asset you would like backed, as well as the amount of WAXP tokens.
-> In the token field, you must provide 8 decimal places, as well as the symbol "WAX". I.e. "25.00000000 WAX" (without quotation marks).

Step 6: Under the "maintenance" action, hit the switch off & submit. While maintenance is on, the assets will not be backed by WAXP tokens, however, assets will continue to be minted.

And that's all that there is to it.

In the case that you would like an asset that is already minted to be backed by WAXP, as long as the "settemplate" data is setup, users can perform the "claimtokens" action to have their asset backed by WAXP at any time.


How to top up the contract's WAXP balance for token backing; minting can fail when there is no balance:
First time setup: <https://wax-test.bloks.io/account/atomicassets?loadContract=true&tab=Actions&account=atomicassets&scope=atomicassets&limit=100&action=announcedepo> call this with contract name and `8,WAX` (to setup your `atomicassets` balances)

<https://wax-test.bloks.io/wallet/transfer> Sent to: `atomicassets` Memo: `deposit` (you can only send WAX)

To withdraw: <https://wax-test.bloks.io/account/atomicassets?loadContract=true&tab=Actions&account=atomicassets&scope=atomicassets&limit=100&action=withdraw>


Action Info:

Action: `init`, call it once/first time after uploading contract
Action: `destruct`, will remove the config from the contract (eg for config/contract upgrades)
Action: `maintenance`, toggle maintenance on/off, by default maintenance is on. Maintenance = no tokens will be backed, asset minting will be OK
Action: `settemplate`, update or set the config for a template id; use full token precision eg `1.12341234 WAX`
Action: `rmtemplate`, remove a template
Action: `claimtokens`, allows users to let the contract back their asset (if allowed) with tokens


Disclaimer: This smart contract has been developed & open sourced in good faith. It has been used to back over 500k WAXP for assets in the "terraformers" collection.
By following the steps, using this contract or using the open source code provided, you are waiving all liability from us & you are acknowledging that you are proceeding at your own risk. We do not take any responsibility for any harm.


Socials:
Dapp @ <https://tfmw.gg>
Twitter @ <https://twitter.com/TFMW_gg>
Discord @ <https://discord.gg/terraformers>


Contacts via Discord:
on a t break @ on a t break#6196
TheLonely @ TheLonely#4812
