var aws = require('aws-sdk');
var dynamodb = new aws.DynamoDB();
// Update the Table name below
var tablename = process.env.TABLE_NAME;

exports.scandb = (event, context, callback) => {
    dynamodb.scan({
        TableName: tablename
    }, (err, data) => {
        callback(null, data['Items']);
    });
};

exports.insertitem = (event, context, callback) => {
    var params = {
        TableName: tablename,
        Item: {
            "payload": {
                "M": {
                    "location": {
                        "L": [{
                                "N": "-31.99588"
                            },
                            {
                                "N": "115.908"
                            }
                        ]
                    },
                    "ssid": {
                        "M": {
                            "channel": {
                                "N": "6"
                            },
                            "rssi": {
                                "N": "-19"
                            },
                            "ssid": {
                                "S": "HomeNet"
                            }
                        }
                    }
                }
            },
            "ssid": {
                "S": "HomeNet"
            },
            "timestamp": {
                "N": "1538917579082"
            }
        }
    };

    dynamodb.putItem(params, function (err, data) {
        if (err) {
            console.log("Error", err);
        } else {
            console.log("Success", data);
        }
    });
};
