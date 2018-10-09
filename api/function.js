var aws = require('aws-sdk');
var dynamodb = new aws.DynamoDB();
// Update the Table name below
var tablename = process.env.TABLE_NAME ;

exports.scandb = (event, context, callback) => {
    dynamodb.scan({TableName: tablename}, (err, data) => {
        callback(null, data['Items']);
    });
};

exports.insertitem = (event, context, callback) => {
    var params = {
    TableName: tablename,
    Item:{
         "PriKey": { "S": "StarWars1" },
            "Column2": { "S": "Skywalker" },
            "Column3": { "S": "DarthVader" },
            "Column4": { "S": "Yoda"}
        }
    };

    dynamodb.putItem(params, function(err, data) {
        if (err) {
            console.log("Error", err);
                } else {
            console.log("Success", data);
                }
        });
};
