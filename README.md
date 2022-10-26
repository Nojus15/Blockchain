# Blockchain

## Usage

`./app`

### Main run commands

| Command             | Description                                                                |
|---------------------|----------------------------------------------------------------------------|
| --gu {count}        | Generated file with users that have random name, public key and balance    |
| --gTx {count}       | Generates random transactions with generated users and outputs to a file   |
| --mine              | Starts mining proccess and mines until no transactions are left            |

### Block mining

Each mined block is printed to the terminal.
Nonce value with current hash guess is displayed in between the mined blocks.
After all blocks are mined they are printed to a file.

### Commands for exploring mined blocks

| Command                      | Description                    |
|------------------------------|--------------------------------|
| --getBlockCount              | Returns mined blocks count     |
| --getBlockInfo {number}      | Returns selected block info    |
