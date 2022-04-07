const res = require("express/lib/response");
const { json } = require("express/lib/response");
const { parse } = require("pg-protocol");
const pool = require("../../db");
const qurries = require("./qurey");


const getAllSensorData = (req,res) => {
    pool.query(qurries.getDataforSensor, (error,results) => {
        if (error) throw error;
        res.status(200).json(results.rows);
    });
};

const getAllSensorDataUserWise = (req,res) => {
    
    let sUserName=req.params.id;
    console.log("Query String: "+ sUserName);
    pool.query(qurries.getAllSensorData,[sUserName], (error,results) => {
        if (error) throw error;
        res.status(200).json( results.rows);
    });
};



const addSensorData = (req,res) => {
    console.log(req.body);
    const JsonData=JSON.parse(JSON.stringify(req.body));
    
        let current = new Date();
        let cDate = current.getFullYear() + '-' + (current.getMonth() + 1) + '-' + current.getDate();
        let cTime = current.getHours() + ":" + current.getMinutes() + ":" + current.getSeconds();
        let dateTime = cDate + ' ' + cTime;

        console.log("CurrentDate "+dateTime);
         pool.query(qurries.addSensorData, [JsonData.username,JsonData.email_id,JsonData.devicename,JsonData.temperature,JsonData.humidity,dateTime], (error,results) =>{
             if(error) throw error;
             res.status(201).send("sensor data inserter!!");
         }
     );
};
module.exports= {
    getAllSensorData,
    addSensorData,
    getAllSensorDataUserWise
};