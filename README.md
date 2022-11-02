# Blockchain

## Usage

`./app`

### Main run commands

| Command              | Description                                                                |
|----------------------|----------------------------------------------------------------------------|
| --gu {count}         | Generated file with users that have random name, public key and balance    |
| --gTx {count}        | Generates random transactions with generated users and outputs to a file   |
| --mine {threadCount} | Starts mining proccess and mines until no transactions are left            |

### Main run commands

To run simulation you first have to create users file, then transactions file, and only then start the mining process.
Users balances are updated to the file after the mining is complete. Blocks are appended to the "blocks.txt" file after every block
to save some RAM.

### Block mining

Each mined block is printed to the terminal.
Nonce value with current hash guess is displayed in between the mined blocks.
After all blocks are mined they are printed to a file.

### Commands for exploring mined blocks

| Command                                                | Description                                             |
|--------------------------------------------------------|---------------------------------------------------------|
| --getBlockCount                                        | Returns mined blocks count                              |
| --getBlockInfo {number}                                | Returns selected block info                             |
| --getBeshHashBlock                                     | Returns block containting smallest hash info            |
| --getHardestBlock                                      | Returns clock with biggest nonce value                  |
| --getAverageDifficulty                                 | Returns average block difficulty                        |
| --getBlockTransaction {blockPos} {transactionPos}      | Returns selected transaction info from a selected block |
