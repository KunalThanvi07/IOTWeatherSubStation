
// used for example purposes
// function getRandomIntInclusive(min, max) {
//     min = Math.ceil(min);
//     max = Math.floor(max);
//     return Math.floor(Math.random() * (max - min + 1)) + min;
//   }


// create initial empty chart
var ctx_live = document.getElementById("mycanvas");
var myChart = new Chart(ctx_live, {
    type: 'line',
    data: {
        labels: [],
        datasets: [{
            data: [],
            borderWidth: 1,
            borderColor: '#00c0ef',
            label: 'temperature',
            yAxisID: 'y'
            
        },{
            data: [],
            borderWidth: 1,
            borderColor: '#ffc0ef',
            label: 'humidity',
            yAxisID: 'y',
        }
        ]
    },
    // options: {
    //     responsive: true,
    //     title: {
    //         display: true,
    //         text: "Chart.js - Dynamically Update Chart Via Ajax Requests",
    //     },
    options: {
        responsive: true,
        plugins: {
            title: {
                display: true,
                text: 'Chhattisgarh'
            }
        },
        legend: {
            display: false
        },
        scales: {
           
            y: {
                type: 'linear',
                display: true,
                position: 'left',
                min:10,
                max:100
            }
        }
    }
});

// this post id drives the example data
var postId = 1;


// logic to get new data
var getData = function () {
    var settings = {
        'cache': true,
        'dataType': "json",
        "async": true,
        "crossDomain": true,
        "url": "https://myweatherappiot.herokuapp.com/api/v1/sensor/user/milind",
        "method": "GET",
        "headers": {
            "accept": "application/json",
            "Access-Control-Allow-Origin": "*"
        }
    }



    $.ajax(settings).done(function (data) {
        // process your data to pull out what you plan to use to update the chart
        // e.g. new label and a new data point

        // add new label and data point to chart's underlying data structures


        //  var JSONData=json.parse(json.stringfy(data));   

        var myJSON = JSON.parse(JSON.stringify(data));
       // $("#jsondisp").text(myJSON[0].username);

        $.each(myJSON, function (bb) {
            //  console.log (bb);
            console.log(myJSON[bb]);
            // console.log (myJSON[bb].id);
           // clearArray( myChart.data.datasets);
           // clearArray( myChart.data.labels);

            myChart.data.labels.push("Temp " +myJSON[bb].temperature);
            myChart.data.datasets[0].data.push(myJSON[bb].temperature);
            myChart.data.datasets[1].data.push(myJSON[bb].humidity);
             
            myChart.update();
        });

        //console.log(myJSON);
        //  myChart.data.labels.push("Post " + postId++);
        //  myChart.data.datasets[0].data.push(getRandomIntInclusive(1, 25));
        //  myChart.data.datasets[1].data.push(getRandomIntInclusive(5, 10));

        // // // re-render the chart
        //  myChart.update();
    });

}
//getData();
  // get new data every 3 seconds
  setInterval(getData, 5000);
