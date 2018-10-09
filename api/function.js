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
                    "lng": {
                        "S": "115.910812"
                    },
                    "ssid": {
                        "M": {
                            "channel": {
                                "N": "1"
                            },
                            "ssid": {
                                "S": "Nic"
                            },
                            "rssi": {
                                "N": "-92"
                            }
                        }
                    },
                    "lat": {
                        "S": "-31.993866"
                    }
                }
            },
            "topic": {
                "S": "hackathons/aws-devpost/gps-glover-01"
            },
            "timestamp": {
                "N": "1538911294993"
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
