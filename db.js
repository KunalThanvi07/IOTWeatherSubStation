const Pool = require("pg").Pool

// For Local Development

// const pool = new Pool({
//     host : "localhost",
//     user : "postgres",
//     database : "TempDB_Demo",
//     password : "Kunal@123",
//     port : 5432
// });


//HeroKu connection string 
const connectionString = 'postgres://opptrrrrxgzrnh:9d3fff797c5e4ba6070848e57c45f515865fc8d4cc0f0c7a5488be0cb94132c5@ec2-54-157-160-218.compute-1.amazonaws.com:5432/dfe4unk6fcsih3'
const pool = new Pool({
  connectionString,
})

module.exports=pool;
