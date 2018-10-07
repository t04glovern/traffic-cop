# Traffic Cop

## Proof of Concept

![img/poc-circuit.jpg](img/poc-circuit.jpg)

## Deploying CF

### IoT Vending Machine

Deploys a IoT Vending machine instances that can be used to generate certificates for new devices

```bash
aws cloudformation create-stack --stack-name "TrafficCop-IoT-Vending-Machine" \
--template-body file://aws/Iot-Certificate-Vending-Machine.json \
--parameters file://aws/Iot-Certificate-Vending-Machine-Params.json \
--capabilities CAPABILITY_IAM
```

#### Create Device

Check the outputs from your cloudformation stack and retrieve the *RequestUrlExample*, It'll look like the following:

```bash
https://<api-gateway-id>.execute-api.us-east-1.amazonaws.com/LATEST/getcert?serialNumber=value1&deviceToken=value2
```

Create a new item in the DynamoDB instance that was created by the previous CloudFormation script (the DB is called deviceInfo)

![img/dynamodb-deviceInfo.png](img/dynamodb-deviceInfo.png)

* **serialNumber**: This should be something unique and will be the identifier for the IoT device you are onboarding (*e.g. gps-glover-01*)
* **deviceToken**: This should be a private hash/secret that you will use to generate certificates for your new device securely (*e.g. 572589798725*)

Substitute these two into your API Gateway query

```bash
https://<api-gateway-id>.execute-api.us-east-1.amazonaws.com/LATEST/getcert?serialNumber=gps-glover-01&deviceToken=572589798725
```

You'll be returned a json response:

```json
{
    "certificateArn": "arn:aws:iot:us-east-1:<account-id>:cert/009ff6ee0.........",
    "certificateId": "009ff6ee092e......",
    "certificatePem": "-----BEGIN CERTIFICATE-----\nMIIDWTCCAkGgAwIBAgIUZiIgLi......-----END CERTIFICATE-----\n",
    "keyPair": {
        "PublicKey": "-----BEGIN PUBLIC KEY-----\nMIIBIjANBgkqhkiG9w0BAQEFAAO.......-----END PUBLIC KEY-----\n",
        "PrivateKey": "-----BEGIN RSA PRIVATE KEY-----\nMIIEpAIBAAKCAQ........-----END RSA PRIVATE KEY-----\n"
    },
    "RootCA": "-----BEGIN CERTIFICATE-----\r\nMIIE0zCCA7ugAwIBAgIQGNrRniZ96Lt........-----END CERTIFICATE-----"
}
```

Place the outputs for each of the three fields below into new files in [esp8266-gps/certs](esp8266-gps/certs)

* **esp8266-gps/certs/certificate.pem.crt**: certificatePem
* **esp8266-gps/certs/private.pem.key**: keyPair.PrivateKey
* **esp8266-gps/certs/root-CA.pem**: RootCA

Follow the instructions in [esp8266-gps/README.md](esp8266-gps/README.md) on how to convert and upload these certificates to the ESP8266

### DynamoDB Instance

Deploys the DynamoDB table + an action rule to push data from a topic into said Dynamo instance

```bash
aws cloudformation create-stack --stack-name "TrafficCop-IoT-TopicRule" \
--template-body file://aws/IoT-TopicRule.json \
--parameters file://aws/IoT-TopicRule-Params.json \
--capabilities CAPABILITY_IAM
```
