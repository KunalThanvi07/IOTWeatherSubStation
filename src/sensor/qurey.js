
// const getSensorData = "select devicename,username,max(datetime) as sensortime from sensordata
// group by username,devicename";

const getDataforSensor = 'select * from public."vwGetAllLatestSensorData"';
const getAllSensorData = "select * from sensordata where username = $1  order by id desc fetch first 1 rows only";
const addSensorData = "insert into sensordata (username,email_id,devicename,temperature,humidity,datetime) values ($1,$2,$3,$4,$5,$6);";


module.exports = {
    getAllSensorData,
    getDataforSensor,
    addSensorData
 };