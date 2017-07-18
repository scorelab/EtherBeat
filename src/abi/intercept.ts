import * as Web3 from "web3";
import web3 from "../config/web3conn";

interface IContractProperties {
    function: string;
    type: Web3.AbiIOParameter[];
    result: any;
}

export class AbiContract {
    address: string;
    abiInterface: Array<Web3.AbiDefinition>;
    contractInstance: any;

    constructor(address: string, abiInterfaceString: string) {
        this.address = address;
        this.abiInterface = JSON.parse(abiInterfaceString);

        const MyContract = web3.eth.contract(this.abiInterface);
        this.contractInstance = MyContract.at(this.address);
    }

    getContractProperties() {
        const results: Array<IContractProperties> = [];
        const prps = this.abiInterface.filter((value) => {
            return (value.inputs && value.inputs.length == 0 && value.constant && value.type == "function");
        });

        prps.forEach(func => {
            console.log(func);
            results.push({ function: func.name, type: func.outputs, result: this.contractInstance[func.name]() });
        });

        return results;
    }

    getContractMembers() {

    }
}