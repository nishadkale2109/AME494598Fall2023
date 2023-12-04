var express = require("express");
var app = express();
var bodyParser = require("body-parser");
var errorHandler = require("errorhandler");
var methodOverride = require("method-override");
var MS = require("mongoskin");
var hostname = process.env.HOSTNAME || "localhost";
var port = 8080;

var motion_time;

var db = MS.db("mongodb://127.0.0.1:27017/sensorData");

app.get("/", function (req, res) {
  res.redirect("index.html");
});

app.get("/sendData", function (req, res) {
  motion_time = req.query.motion_time;
  req.query.currentDate = new Date();
  console.log("Hello the server is being called!");
  console.log(req.query);

  db.collection("data").insertOne(req.query, function (result, err) {
    console.log(result, err);
    res.send("1");
  });
});

app.get("/getData", function (req, res) {
  var startDate = req.query.startDate;
  var endDate = req.query.endDate;

  var query = {};
  if (startDate !== "undefined" && endDate !== "undefined") {
    query = {
      currentDate: {
        $gte: new Date(startDate),
        $lte: new Date(endDate),
      },
    };
  }

  db.collection("data")
    .find(query)
    .sort({ currentDate: -1 })
    .toArray(function (err, result) {
      if (err) {
        console.log(err);
        res.status(500).send("Internal Server Error");
      } else {
        console.log(result);
        res.send(JSON.stringify(result));
      }
    });
});

app.use(methodOverride());
app.use(bodyParser());
app.use(express.static(__dirname + "/public"));
app.use(errorHandler());

console.log(
  "Simple static server listening at http://" + hostname + ":" + port
);
app.listen(port);
