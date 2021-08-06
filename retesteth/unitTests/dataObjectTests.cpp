/*
    This file is part of cpp-ethereum.

    cpp-ethereum is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    cpp-ethereum is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with cpp-ethereum.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file dataObjectTests.cpp
 * Unit tests for TestHelper functions.
 */

#include <dataObject/ConvertFile.h>
#include <dataObject/DataObject.h>
#include <retesteth/TestOutputHelper.h>
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace dev;
using namespace test;
using namespace dataobject;

BOOST_FIXTURE_TEST_SUITE(DataObjectTestSuite, TestOutputHelperFixture)

BOOST_AUTO_TEST_CASE(dataobject_bracers)
{
    string data = R"(
      {
      "logs": [
          {
           "address": "0x0f572e5295c57f15886f9b263e2f6d2d6c7b5ec6",
           "topics": [
            "0x000000000000000000000000095e7baea6a6c7c4c2dfeb977efac326af552d87"
           ],
           "logIndex": "0x0",
           "removed": false
          }
         ]
      }
    )";

    try
    {
        spDataObject a = ConvertJsoncppStringToData(data);
        BOOST_REQUIRE(a->atKey("logs").at(0).atKey("removed").asBool() == false);
    }
    catch (DataObjectException const& _ex)
    {
        BOOST_ERROR(
            "Unexpected DataObject exception when parsing json: " + std::string(_ex.what()));
    }
}


BOOST_AUTO_TEST_CASE(dataobject_EscapeChars)
{
    string data = R"(
        {
                  "jsonrpc":"2.0",
                  "id":14,
                  "error":{
                    "code":-32602,
                    "message":"invalid argument 0: invalid hex or decimal integer \"0x\""
                  }
        }
    )";
    try
    {
        spDataObject a = ConvertJsoncppStringToData(data);
        BOOST_REQUIRE(a->atKey("error").atKey("message").asString() ==
                      "invalid argument 0: invalid hex or decimal integer \\\"0x\\\"");
    }
    catch (DataObjectException const&)
    {
        BOOST_ERROR("Unexpected DataObject exception when parsing json!");
    }
}


BOOST_AUTO_TEST_CASE(dataobject_invalidJson1)
{
    string data = R"(
        {
            "name" : {
        }
    )";
    try
    {
        ConvertJsoncppStringToData(data);
    }
    catch (DataObjectException const&)
    {
        return;
    }
    BOOST_ERROR("Expected DataObject exception when parsing json!");
}

BOOST_AUTO_TEST_CASE(dataobject_invalidJson2)
{
    string data = R"(
        {
            "name" : {]
        }
    )";
    try
    {
        ConvertJsoncppStringToData(data);
    }
    catch (DataObjectException const&)
    {
        return;
    }
    BOOST_ERROR("Expected DataObject exception when parsing json!");
}

BOOST_AUTO_TEST_CASE(dataobject_invalidJson3)
{
    string data = R"(
        [
            "name" : {}
        ]
    )";
    try
    {
        ConvertJsoncppStringToData(data);
    }
    catch (DataObjectException const&)
    {
        return;
    }
    BOOST_ERROR("Expected DataObject exception when parsing json!");
}

BOOST_AUTO_TEST_CASE(dataobject_invalidJson4)
{
    string data = R"(
        {
            "name" : "value" : "attempt another value"
        }
    )";
    try
    {
        ConvertJsoncppStringToData(data);
    }
    catch (DataObjectException const&)
    {
        return;
    }
    BOOST_ERROR("Expected DataObject exception when parsing json!");
}

BOOST_AUTO_TEST_CASE(dataobject_invalidJson5)
{
    string data = R"(
        {
            "name" : [ "element" ] : "attempt another value"
        }
    )";
    try
    {
        ConvertJsoncppStringToData(data);
    }
    catch (DataObjectException const&)
    {
        return;
    }
    BOOST_ERROR("Expected DataObject exception when parsing json!");
}

BOOST_AUTO_TEST_CASE(dataobject_readJson1)
{
    string data = R"(
        {
            "name" : {
            }
        }
    )";
    spDataObject dObj = ConvertJsoncppStringToData(data);
    BOOST_CHECK(dObj->asJson(0, false) == "{\"name\":{}}");
}

BOOST_AUTO_TEST_CASE(dataobject_readJson2)
{
    string data = R"(
        {
            "name" : {
            }
        }
    )";
    spDataObject dObj = ConvertJsoncppStringToData(data);
    BOOST_CHECK(dObj->asJson(0, false) == "{\"name\":{}}");
}

BOOST_AUTO_TEST_CASE(dataobject_readJson3)
{
    string data = R"(
        {
            "name" : {
                "key" : [
                ]
            }
        }
    )";

    spDataObject dObj = ConvertJsoncppStringToData(data);
    BOOST_CHECK(dObj->asJson(0, false) == "{\"name\":{\"key\":[]}}");
}

BOOST_AUTO_TEST_CASE(dataobject_readJson4)
{
    string data = R"(
        {
            "name" : {
                "key" : [
                    "aaa",
                    "bbb"
                ]
            }
        }
    )";

    spDataObject dObj = ConvertJsoncppStringToData(data);
    BOOST_CHECK(dObj->asJson(0, false) == "{\"name\":{\"key\":[\"aaa\",\"bbb\"]}}");
}

BOOST_AUTO_TEST_CASE(dataobject_readJson5)
{
    string data = R"(
        {
            "name" : {
                "key" : [
                    12,
                    34
                ]
            }
        }
    )";

    spDataObject dObj = ConvertJsoncppStringToData(data);
    BOOST_CHECK(dObj->asJson(0, false) == "{\"name\":{\"key\":[12,34]}}");
}

BOOST_AUTO_TEST_CASE(dataobject_readJson5b)
{
    string data = R"(
        {
            "name" : {
                "key" : [
                    12,
                    34
                ],
                "key2" : [
                    12,
                    34
                ]
            },
            "name2" : "value"
        }
    )";

    spDataObject dObj = ConvertJsoncppStringToData(data, "key");
    BOOST_CHECK(dObj->asJson(0, false) == "{\"name\":{\"key\":[12,34]}}");
}

BOOST_AUTO_TEST_CASE(dataobject_readJson6)
{
    string data = R"(
        {
            "name" : {
                "key" : -123
            }
        }
    )";

    spDataObject dObj = ConvertJsoncppStringToData(data);
    BOOST_CHECK(dObj->asJson(0, false) == "{\"name\":{\"key\":-123}}");
}

BOOST_AUTO_TEST_CASE(dataobject_readJson7)
{
    string data = R"(
        {
    "name" : {
        "_info" : {
            "comment" : "A tesesult",
            "filledwith" : "testee5c"
        },
        "blocks" : [
            {
                "blockHeader" : {
                    "difficulty" : "0x020000",
                    "extraData" : ""
                },
                "rlp" : "0xf90262f0",
                "transactions" : [
                    {
                        "data" : "0x",
                        "value" : "0x0186a0"
                    }
                ],
                "uncleHeaders" : [
                ]
            }
        ],
        "genesisBlockHeader" : {
            "number" : "0x00"
        },
        "genesisRLP" : "0xf9c6f04171167ec0c0",
        "lastblockhash" : "0x00606595b80acde5",
        "network" : "Homestead",
        "postState" : {
            "0x095e7baea6a6c7c4c2dfeb977efac326af552d87" : {
                "storage" : {
                    "0x00" : "0x02"
                }
            },
            "0x2adc25665018aa1fe0e6bc666dac8fc2697ff9ba" : {
                "storage" : {
                }
            },
            "0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b" : {
                "nonce" : "0x01",
                "storage" : {
                }
            }
        },
        "pre" : {
            "0x095e7baea6a6c7c4c2dfeb977efac326af552d87" : {
            },
            "0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b" : {
            }
        },
        "sealEngine" : "NoProof"
    }
        }
    )";

    string const res =
        R"({"name":{"_info":{"comment":"A tesesult","filledwith":"testee5c"},"blocks":[{"blockHeader":{"difficulty":"0x020000","extraData":""},"rlp":"0xf90262f0","transactions":[{"data":"0x","value":"0x0186a0"}],"uncleHeaders":[]}],"genesisBlockHeader":{"number":"0x00"},"genesisRLP":"0xf9c6f04171167ec0c0","lastblockhash":"0x00606595b80acde5","network":"Homestead","postState":{"0x095e7baea6a6c7c4c2dfeb977efac326af552d87":{"storage":{"0x00":"0x02"}},"0x2adc25665018aa1fe0e6bc666dac8fc2697ff9ba":{"storage":{}},"0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b":{"nonce":"0x01","storage":{}}},"pre":{"0x095e7baea6a6c7c4c2dfeb977efac326af552d87":{},"0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b":{}},"sealEngine":"NoProof"}})";

    spDataObject dObj = ConvertJsoncppStringToData(data);
    BOOST_CHECK(dObj->asJson(0, false) == res);
}

BOOST_AUTO_TEST_CASE(dataobject_readJson8)
{
    string data = R"(
        {
            "code" :"pragma solidity ^0.4.0;",
            "code" :"contract suicideContract",
            "code" :"{",
            "code" :"    function run",
            "code" :"    {",
            "code" :"        suicide 0x1000000000000000000000000000000000000001;",
            "code" :"    }",
            "code" :"}"
        }
    )";

    spDataObject dObj = ConvertJsoncppStringToData(data);
    BOOST_CHECK(dObj->asJson(0, false) == "{\"code\":\"}\"}");
}

BOOST_AUTO_TEST_CASE(dataobject_readJson9)
{
    string data = R"(
    {
        "array" : [
           1, 2, -1, 5, 3
        ]
    }
    )";

    spDataObject dObj = ConvertJsoncppStringToData(data);
    BOOST_CHECK(dObj->asJson(0, false) == "{\"array\":[1,2,-1,5,3]}");
}

BOOST_AUTO_TEST_CASE(dataobject_readJson10)
{
    string data = R"(
    {
        "array" : [
           "1", "2", "-1", "5", "3"
        ]
    }
    )";

    spDataObject dObj = ConvertJsoncppStringToData(data);
    BOOST_CHECK(dObj->asJson(0, false) == "{\"array\":[\"1\",\"2\",\"-1\",\"5\",\"3\"]}");
}

BOOST_AUTO_TEST_CASE(dataobject_readJson11)
{
    string data = R"(
{
    "name" : "Ethereum GO on TCP",
    "socketType" : "tcp",
    "socketAddress" : [
                "127.0.0.1:8545",
                "127.0.0.1:8552"
        ],
        "forks" : [
                "Frontier",
                "ConstantinopleFix"
        ]
}
    )";

    spDataObject dObj = ConvertJsoncppStringToData(data);
    BOOST_CHECK(dObj->count("forks"));
}

BOOST_AUTO_TEST_CASE(dataobject_readJson12)
{
    string data = R"(
    {
        "Byzantium" : [
            {
                "hash" : "0x",
                "indexes" : {
                    "data" : 0,
                    "value" : 0
                 }
            }
        ],
        "Constantinople" : [
            {
                "hash" : "0x",
                "indexes" : {
                    "data" : 0,
                    "value" : 0
                 }
            }
        ]
    }
    )";

    spDataObject dObj = ConvertJsoncppStringToData(data);
    string const res =
        R"({"Byzantium":[{"hash":"0x","indexes":{"data":0,"value":0}}],"Constantinople":[{"hash":"0x","indexes":{"data":0,"value":0}}]})";
    BOOST_CHECK(dObj->asJson(0, false) == res);
}

BOOST_AUTO_TEST_CASE(dataobject_readJson13)
{
    string data = R"(
    {
       "expect" : [
        {
              "result" : {
                  "0x0f572e5295c57f15886f9b263e2f6d2d6c7b5ec6" : {
                      "storage" : {
                          "0x02" : "0"
                      }
                  },
                  "0x0f572e5295c57f15886f9b263e2f6d2d6c7b5ec6" : {
                  }
             }
        },
        {
              "result" : {
                  "0xd27e800c69122409ac5609fe4df903745f3988a0" : {
                       "storage" : {
                          "0x01" : "0x01"
                       }
                  }
             }
        }
                  ]
    }
    )";

    spDataObject dObj = ConvertJsoncppStringToData(data);
    string const res =
        R"({"expect":[{"result":{"0x0f572e5295c57f15886f9b263e2f6d2d6c7b5ec6":{}}},{"result":{"0xd27e800c69122409ac5609fe4df903745f3988a0":{"storage":{"0x01":"0x01"}}}}]})";
    BOOST_CHECK(dObj->asJson(0, false) == res);
}

BOOST_AUTO_TEST_CASE(dataobject_readJson14)
{
    string data = R"( {"jsonrpc":"2.0","id":1,"result":true}   )";
    spDataObject dObj = ConvertJsoncppStringToData(data);
    string res = R"({"jsonrpc":"2.0","id":1,"result":true})";
    BOOST_CHECK(dObj->asJson(0, false) == res);
}

BOOST_AUTO_TEST_CASE(dataobject_readJson15)
{
    string data = R"(
                {
                    "array" : [
                        null
                    ],
                    "object" : null
                }
       )";
    spDataObject dObj = ConvertJsoncppStringToData(data);
    string res = R"({"array":[null],"object":null})";
    BOOST_CHECK(dObj->asJson(0, false) == res);
}


BOOST_AUTO_TEST_CASE(dataobject_findOrderedKeyPosition_before1_of3)
{
    string const key = "aab0";
    DataObject data(DataType::Object);
    data["aab1"] = "1";
    data["abb2"] = "2";
    data["adb3"] = "3";
    size_t pos = dataobject::findOrderedKeyPosition(key, data.getSubObjects());
    BOOST_CHECK(pos == 0);
}

BOOST_AUTO_TEST_CASE(dataobject_findOrderedKeyPosition_before2_of3)
{
    string const key = "abb0";
    DataObject data(DataType::Object);
    data["aab1"] = "1";
    data["abb2"] = "2";
    data["adb3"] = "3";
    size_t pos = dataobject::findOrderedKeyPosition(key, data.getSubObjects());
    BOOST_CHECK(pos == 1);
}

BOOST_AUTO_TEST_CASE(dataobject_findOrderedKeyPosition_before3_of3)
{
    string const key = "acb4";
    DataObject data(DataType::Object);
    data["aab1"] = "1";
    data["abb2"] = "2";
    data["adb3"] = "3";
    size_t pos = dataobject::findOrderedKeyPosition(key, data.getSubObjects());
    BOOST_CHECK(pos == 2);
}

BOOST_AUTO_TEST_CASE(dataobject_findOrderedKeyPosition_after3_of3)
{
    string const key = "adb4";
    DataObject data(DataType::Object);
    data["aab1"] = "1";
    data["abb2"] = "2";
    data["adb3"] = "3";
    size_t pos = dataobject::findOrderedKeyPosition(key, data.getSubObjects());
    BOOST_CHECK(pos == 3);
}

BOOST_AUTO_TEST_CASE(dataobject_findOrderedKeyPosition_after6_of6)
{
    string const key = "v";
    DataObject data(DataType::Object);
    data["d"] = "1";
    data["g"] = "2";
    data["gs"] = "3";
    data["n"] = "4";
    data["s"] = "5";
    data["t"] = "6";
    size_t pos = dataobject::findOrderedKeyPosition(key, data.getSubObjects());
    BOOST_CHECK(pos == 6);
}

BOOST_AUTO_TEST_CASE(dataobject_findOrderedKeyPosition_empty)
{
    string const key = "adb4";
    DataObject data(DataType::Object);
    size_t pos = dataobject::findOrderedKeyPosition(key, data.getSubObjects());
    BOOST_CHECK(pos == 0);
}

BOOST_AUTO_TEST_CASE(dataobject_findOrderedKeyPosition_before1_of8)
{
    string const key = "aa0";
    DataObject data(DataType::Object);
    data["aa1"] = "1";
    data["aa2"] = "2";
    data["aa3"] = "3";
    data["aa4"] = "4";
    data["aa5"] = "5";
    data["aa6"] = "6";
    data["aa7"] = "7";
    data["aa8"] = "8";

    size_t pos = dataobject::findOrderedKeyPosition(key, data.getSubObjects());
    BOOST_CHECK(pos == 0);
}

BOOST_AUTO_TEST_CASE(dataobject_findOrderedKeyPosition_before2_of8)
{
    string const key = "aa1b";
    DataObject data(DataType::Object);
    data["aa1"] = "1";
    data["aa2"] = "2";
    data["aa3"] = "3";
    data["aa4"] = "4";
    data["aa5"] = "5";
    data["aa6"] = "6";
    data["aa7"] = "7";
    data["aa8"] = "8";

    size_t pos = dataobject::findOrderedKeyPosition(key, data.getSubObjects());
    BOOST_CHECK(pos == 1);
}

BOOST_AUTO_TEST_CASE(dataobject_findOrderedKeyPosition_before3_of8)
{
    string const key = "aa2b";
    DataObject data(DataType::Object);
    data["aa1"] = "1";
    data["aa2"] = "2";
    data["aa3"] = "3";
    data["aa4"] = "4";
    data["aa5"] = "5";
    data["aa6"] = "6";
    data["aa7"] = "7";
    data["aa8"] = "8";

    size_t pos = dataobject::findOrderedKeyPosition(key, data.getSubObjects());
    BOOST_CHECK(pos == 2);
}

BOOST_AUTO_TEST_CASE(dataobject_findOrderedKeyPosition_before4_of8)
{
    string const key = "aa3b";
    DataObject data(DataType::Object);
    data["aa1"] = "1";
    data["aa2"] = "2";
    data["aa3"] = "3";
    data["aa4"] = "4";
    data["aa5"] = "5";
    data["aa6"] = "6";
    data["aa7"] = "7";
    data["aa8"] = "8";

    size_t pos = dataobject::findOrderedKeyPosition(key, data.getSubObjects());
    BOOST_CHECK(pos == 3);
}

BOOST_AUTO_TEST_CASE(dataobject_findOrderedKeyPosition_before5_of8)
{
    string const key = "aa4b";
    DataObject data(DataType::Object);
    data["aa1"] = "1";
    data["aa2"] = "2";
    data["aa3"] = "3";
    data["aa4"] = "4";
    data["aa5"] = "5";
    data["aa6"] = "6";
    data["aa7"] = "7";
    data["aa8"] = "8";

    size_t pos = dataobject::findOrderedKeyPosition(key, data.getSubObjects());
    BOOST_CHECK(pos == 4);
}

BOOST_AUTO_TEST_CASE(dataobject_findOrderedKeyPosition_before6_of8)
{
    string const key = "aa5b";
    DataObject data(DataType::Object);
    data["aa1"] = "1";
    data["aa2"] = "2";
    data["aa3"] = "3";
    data["aa4"] = "4";
    data["aa5"] = "5";
    data["aa6"] = "6";
    data["aa7"] = "7";
    data["aa8"] = "8";

    size_t pos = dataobject::findOrderedKeyPosition(key, data.getSubObjects());
    BOOST_CHECK(pos == 5);
}

BOOST_AUTO_TEST_CASE(dataobject_findOrderedKeyPosition_before7_of8)
{
    string const key = "aa6b";
    DataObject data(DataType::Object);
    data["aa1"] = "1";
    data["aa2"] = "2";
    data["aa3"] = "3";
    data["aa4"] = "4";
    data["aa5"] = "5";
    data["aa6"] = "6";
    data["aa7"] = "7";
    data["aa8"] = "8";

    size_t pos = dataobject::findOrderedKeyPosition(key, data.getSubObjects());
    BOOST_CHECK(pos == 6);
}

BOOST_AUTO_TEST_CASE(dataobject_findOrderedKeyPosition_before8_of8)
{
    string const key = "aa7b";
    DataObject data(DataType::Object);
    data["aa1"] = "1";
    data["aa2"] = "2";
    data["aa3"] = "3";
    data["aa4"] = "4";
    data["aa5"] = "5";
    data["aa6"] = "6";
    data["aa7"] = "7";
    data["aa8"] = "8";

    size_t pos = dataobject::findOrderedKeyPosition(key, data.getSubObjects());
    BOOST_CHECK(pos == 7);
}

BOOST_AUTO_TEST_CASE(dataobject_findOrderedKeyPosition_after8_of8)
{
    string const key = "aa9";
    DataObject data(DataType::Object);
    data["aa1"] = "1";
    data["aa2"] = "2";
    data["aa3"] = "3";
    data["aa4"] = "4";
    data["aa5"] = "5";
    data["aa6"] = "6";
    data["aa7"] = "7";
    data["aa8"] = "8";

    size_t pos = dataobject::findOrderedKeyPosition(key, data.getSubObjects());
    BOOST_CHECK(pos == 8);
}


BOOST_AUTO_TEST_CASE(dataobject_findOrderedKeyPosition_after7_of7)
{
    string const key = "aa8";
    DataObject data(DataType::Object);
    data["aa1"] = "1";
    data["aa2"] = "2";
    data["aa3"] = "3";
    data["aa4"] = "4";
    data["aa5"] = "5";
    data["aa6"] = "6";
    data["aa7"] = "7";

    size_t pos = dataobject::findOrderedKeyPosition(key, data.getSubObjects());
    BOOST_CHECK(pos == 7);
}

BOOST_AUTO_TEST_CASE(dataobject_jsonOrder2)
{
    string const data = R"(
    {
        "balance" : "1000000000000000000",
        "nonce" : "0",
        "//comment1" : "contract Killer {",
        "//comment2" : "  function homicide {",
        "//comment3" : "      suicide(msg.sender;",
        "//comment4" : "    }",
        "//comment5" : "",
        "//comment6" : "    function multipleHomocide {",
        "//comment7" : "        Killer k  = this;",
        "//comment8" : "        k.homicide;",
        "//comment9" : "        k.homicide;",
        "//comment10" : "    }",
        "//comment11" : "",
        "//comment12" : "    function run returns ",
        "//comment13" : "    {",
        "//comment14" : "  multipleHomocide;",
        "//comment15" : "  return true;",
        "//comment16" : "    }",
        "//comment17" : "}",
        "code" : "0x606060405260e060020a600035046309e587a58114610031578063c04062261461004d578063dd4f1f2a1461005a575b005b61002f3373ffffffffffffffffffffffffffffffffffffffff16ff5b6100f5600061010961005e565b61002f5b60003090508073ffffffffffffffffffffffffffffffffffffffff166309e587a56040518160e060020a0281526004018090506000604051808303816000876161da5a03f1156100025750604080517f09e587a500000000000000000000000000000000000000000000000000000000815290516004828101926000929190829003018183876161da5a03f1156100025750505050565b604080519115158252519081900360200190f35b5060019056",
        "storage" : {
        }
    })";
    spDataObject dObj = ConvertJsoncppStringToData(data, string(), true);
    BOOST_CHECK(dObj->at(dObj->getSubObjects().size() - 2).getKey() == "nonce");
}

BOOST_AUTO_TEST_CASE(dataobject_jsonOrder)
{
    DataObject data(DataType::Object);
    data.setAutosort(true);
    data["aa1"] = "1";
    data["aa31"] = "3";
    data["aa3"] = "3";
    data["aa7"] = "7";
    data["aa5"] = "5";
    data["aa2"] = "2";
    data["aa70"] = "7";
    data["aa8"] = "8";
    BOOST_CHECK(data.asJson(0, false) ==
                "{\"aa1\":\"1\",\"aa2\":\"2\",\"aa3\":\"3\",\"aa31\":\"3\",\"aa5\":\"5\",\"aa7\":"
                "\"7\",\"aa70\":\"7\",\"aa8\":\"8\"}");
}

BOOST_AUTO_TEST_CASE(dataobject_replace)
{
    spDataObject data(new DataObject());
    spDataObject data2(new DataObject());
    spDataObject data3(new DataObject());
    (*data2).setKey("key2");
    (*data2).setString("value2");
    (*data3).setKey("key3");
    (*data3).setString("value3");

    (*data).atKeyPointer("field1") = data3; // null object with key "field1" keep the key "field1"
    BOOST_CHECK(data->asJson(0,false) == "{\"field1\":\"value3\"}");

    (*data).atKeyPointer("field1") = data2; // not null object with key "field1" replaces the key "field1" to data2's key
    BOOST_CHECK(data->asJson(0,false) == "{\"key2\":\"value2\"}");
}

BOOST_AUTO_TEST_CASE(dataobject_arrayhell)
{
    string const data = R"(
    {
        "array" : [
                    [
                        {
                            "address" : "0x0000000000000000000000000000000000001337",
                            "storageKeys" : [
                                "0x0000000000000000000000000000000000000000000000000000000000000001",
                                "0x0000000000000000000000000000000000000000000000000000000000000002"
                            ]
                        }
                    ]
                ]
    })";
    spDataObject dObj = ConvertJsoncppStringToData(data, string(), true);
}

BOOST_AUTO_TEST_CASE(dataobject_besuresponse)
{
    string const data = R"(
        {
          "result" : {
            "transactions" : [ {
              "blockHash" : "0xac7b82af234ef01bf4d24a3b9c22c2de091c6f71ec04d51ff23bd780533d999f",
              "blockNumber" : "0x1",
              "chainId" : null,
              "from" : "0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b",
              "gas" : "0x7a120",
              "gasPrice" : "0xa",
              "hash" : "0x225117089dee26945644798e2c64d3117f55c95c7cf5509f7176de4b3af5202d",
              "input" : "0x604b80600c6000396000f3007c01000000000000000000000000000000000000000000000000000000006000350463cbf0b0c08114602d57005b60006004358073ffffffffffffffffffffffffffffffffffffffff16ff",
              "nonce" : "0x0",
              "publicKey" : "0x3a514176466fa815ed481ffad09110a2d344f6c9b78c1d14afc351c3a51be33d8072e77939dc03ba44790779b7a1025baf3003f6732430e20cd9b76d953391b3",
              "raw" : "0xf8a5800a8307a1208081ffb857604b80600c6000396000f3007c01000000000000000000000000000000000000000000000000000000006000350463cbf0b0c08114602d57005b60006004358073ffffffffffffffffffffffffffffffffffffffff16ff1ca00e7d3c664c49aa9f5ce4eb76c8547450466262a78bd093160f492ea0853c68e9a03f843e72210ff1da4fd9e375339872bcf0fad05c014e280ffc755e173700dd62",
              "to" : null,
              "transactionIndex" : "0x0",
              "value" : "0xff",
              "v" : "0x1c",
              "r" : "0xe7d3c664c49aa9f5ce4eb76c8547450466262a78bd093160f492ea0853c68e9",
              "s" : "0x3f843e72210ff1da4fd9e375339872bcf0fad05c014e280ffc755e173700dd62"
            } ]
          }
        })";
    spDataObject dObj = ConvertJsoncppStringToData(data, string(), true);

    string const expectedParse =
        R"({"result":{"transactions":[{"blockHash":"0xac7b82af234ef01bf4d24a3b9c22c2de091c6f71ec04d51ff23bd780533d999f","blockNumber":"0x1","chainId":null,"from":"0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b","gas":"0x7a120","gasPrice":"0xa","hash":"0x225117089dee26945644798e2c64d3117f55c95c7cf5509f7176de4b3af5202d","input":"0x604b80600c6000396000f3007c01000000000000000000000000000000000000000000000000000000006000350463cbf0b0c08114602d57005b60006004358073ffffffffffffffffffffffffffffffffffffffff16ff","nonce":"0x0","publicKey":"0x3a514176466fa815ed481ffad09110a2d344f6c9b78c1d14afc351c3a51be33d8072e77939dc03ba44790779b7a1025baf3003f6732430e20cd9b76d953391b3","r":"0xe7d3c664c49aa9f5ce4eb76c8547450466262a78bd093160f492ea0853c68e9","raw":"0xf8a5800a8307a1208081ffb857604b80600c6000396000f3007c01000000000000000000000000000000000000000000000000000000006000350463cbf0b0c08114602d57005b60006004358073ffffffffffffffffffffffffffffffffffffffff16ff1ca00e7d3c664c49aa9f5ce4eb76c8547450466262a78bd093160f492ea0853c68e9a03f843e72210ff1da4fd9e375339872bcf0fad05c014e280ffc755e173700dd62","s":"0x3f843e72210ff1da4fd9e375339872bcf0fad05c014e280ffc755e173700dd62","to":null,"transactionIndex":"0x0","v":"0x1c","value":"0xff"}]}})";
    BOOST_CHECK(dObj->asJson(0, false) == expectedParse);
}

BOOST_AUTO_TEST_SUITE_END()
