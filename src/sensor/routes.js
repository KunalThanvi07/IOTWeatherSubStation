const { Router } = require("express");
const controller = require("./controller")

const router = Router();


router.get("/", controller.getAllSensorData);

router.post("/", controller.addSensorData );
router.get("/user/:id", controller.getAllSensorDataUserWise);
module.exports= router;  