const express=require('express');
const path = require('path');
const sensorRoutes = require('./src/sensor/routes');
var cors = require('cors')


const app=express();

// html page call
console.log(path.join(__dirname, '/html'));
const StaticPath = path.join(__dirname, '/html');

app.use(express.json());
app.use(cors());
app.use(express.static(StaticPath));

app.use((req, res, next) => {
    res.append('Access-Control-Allow-Origin', ['*']);
    res.append('Access-Control-Allow-Methods', 'GET,PUT,POST,DELETE');
    res.append('Access-Control-Allow-Headers', 'Content-Type');
    next();
});

app.get("/", (req,res) => {
    res.send("Hello!!");
});


app.use("/api/v1/sensor",sensorRoutes);

var server_port = process.env.YOUR_PORT || process.env.PORT || 80;
var server_host = process.env.YOUR_HOST || '0.0.0.0';


app.listen(server_port, server_host, function() {
    console.log('Listening on port %s:%d',server_host, server_port);
});

