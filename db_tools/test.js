var aws = require('aws-sdk');
var dynamodb = new aws.DynamoDB({
    region: "us-east-1"
});

var tablename = "TrafficCopLocations";

insertitem = (event, context, callback) => {
    var params = {
        TableName: tablename,
        Item: {
            "payload": {
                "M": {
                    "location": {
                        "L": [{
                                "N": "-31.99388"
                            },
                            {
                                "N": "115.9108"
                            }
                        ]
                    },
                    "ssid": {
                        "M": {
                            "channel": {
                                "N": "12"
                            },
                            "rssi": {
                                "N": "-90"
                            },
                            "ssid": {
                                "S": "Sauc3"
                            }
                        }
                    }
                }
            },
            "timestamp": {
                "N": "1538917567782"
            },
            "topic": {
                "S": "hackathons/aws-devpost/gps-glover-01"
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

insertitem();
