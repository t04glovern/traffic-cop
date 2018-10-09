let AWS = require("aws-sdk");
let docClient = new AWS.DynamoDB.DocumentClient({
  region: "us-east-1"
});

var fs = require('fs');
var itemJson = JSON.parse(fs.readFileSync('sample_data.json', 'utf8'));

function itemToTable(item) {
    let params = {
      TableName: "TrafficCopLocations",
      Item: item
    };

    return docClient.put(params).promise()
  };

  itemJson.forEach(item => {
    itemToTable(item);
  });
