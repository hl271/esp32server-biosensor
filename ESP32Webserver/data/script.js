// Complete project details: https://randomnerdtutorials.com/esp8266-nodemcu-plot-readings-charts-multiple/

// Get current sensor readings when the page loads
window.addEventListener('load', getReadings);

// Create Temperature Chart
var chart_R = new Highcharts.Chart({
  chart:{
    renderTo:'Rotary Encoder'
  },
  series: [
    {
      name: 'Angle',
      type: 'line',
      color: '#101D42',
      marker: {
        symbol: 'circle',
        radius: 3,
        fillColor: '#101D42',
      }
    }
 
  ],
  title: {
    text: undefined
  },
  xAxis: {
    type: 'datetime',
    dateTimeLabelFormats: { second: '%H:%M:%S' }
  },
  yAxis: {
    title: {
      text: 'Angle Degrees'
    }
  },
  credits: {
    enabled: false
  }
});

var chart_F = new Highcharts.Chart({
  chart:{
    renderTo:'Force Chart'
  },
  series: [
    {
      name: 'Force',
      type: 'line',
      color: '#8B2635',
      marker: {
        symbol: 'triangle',
        radius: 3,
        fillColor: '#8B2635',
      }
    }
 
  ],
  title: {
    text: undefined
  },
  xAxis: {
    type: 'datetime',
    dateTimeLabelFormats: { second: '%H:%M:%S' }
  },
  yAxis: {
    title: {
      text: 'Newton (N)'
    }
  },
  credits: {
    enabled: false
  }
});

var angle = new RadialGauge({
  renderTo: 'Angle',
  width: 300,
  height: 300,
  units: "Degree (o)",
  minValue: -360,
  maxValue: 360,
  colorValueBoxRect: "#049faa",
  colorValueBoxRectEnd: "#049faa",
  colorValueBoxBackground: "#f1fbfc",
  valueInt: 2,
  majorTicks: [
      "-360",
	  "-300",
	  "-240",
	  "-180",
	  "-120",
	  "-60",
	  "0",
      "60",
      "120",
      "240",
      "300",
      "360"

  ],
  minorTicks: 4,
  strokeTicks: true,
  highlights: [
      {
          "from": 80,
          "to": 100,
          "color": "#03C0C1"
      }
  ],
  colorPlate: "#fff",
  borderShadowWidth: 0,
  borders: false,
  needleType: "line",
  colorNeedle: "#007F80",
  colorNeedleEnd: "#007F80",
  needleWidth: 2,
  needleCircleSize: 3,
  colorNeedleCircleOuter: "#007F80",
  needleCircleOuter: true,
  needleCircleInner: false,
  animationDuration: 1500,
  animationRule: "linear"
}).draw();

var force = new LinearGauge({
  renderTo: 'Force',
  width: 120,
  height: 400,
  units: "Newton (N)",
  minValue: 0,
  startAngle: 90,
  ticksAngle: 180,
  maxValue: 40,
  colorValueBoxRect: "#049faa",
  colorValueBoxRectEnd: "#049faa",
  colorValueBoxBackground: "#f1fbfc",
  valueDec: 2,
  valueInt: 2,
  majorTicks: [
      "0",
      "5",
      "10",
      "15",
      "20",
      "25",
      "30",
      "35",
      "40"
  ],
  minorTicks: 4,
  strokeTicks: true,
  highlights: [
      {
          "from": 30,
          "to": 40,
          "color": "rgba(200, 50, 50, .75)"
      }
  ],
  colorPlate: "#fff",
  colorBarProgress: "#CC2936",
  colorBarProgressEnd: "#049faa",
  borderShadowWidth: 0,
  borders: false,
  needleType: "arrow",
  needleWidth: 2,
  needleCircleSize: 7,
  needleCircleOuter: true,
  needleCircleInner: false,
  animationDuration: 1500,
  animationRule: "linear",
  barWidth: 10,
}).draw();

//Plot temperature in the temperature chart
function plot(jsonValue) {

  var keys = Object.keys(jsonValue);
  console.log(keys);
  console.log(keys.length);

  for (var i = 0; i < keys.length; i++){
    var x = (new Date().getTime()+7;
    console.log(x);
    const key = keys[i];
    var y = Number(jsonValue[key]);
    console.log(y);

    if(chart_R.series[i].data.length > 40) {
      chart_R.series[i].addPoint([x, y], true, true, true);
    } else {
      chart_R.series[i].addPoint([x, y], true, false, true);
    }

  }
}

// Function to get current readings on the webpage when it loads for the first time
function getReadings(){
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
	  var myObj = JSON.parse(this.responseText);
      console.log(this.responseText);
	  var ang = myObj.sensor1;
	  angle.value = ang;
    }
  };
  xhr.open("GET", "/readings", true);
  xhr.send();
}

if (!!window.EventSource) {
  var source = new EventSource('/events');

  source.addEventListener('open', function(e) {
    console.log("Events Connected");
  }, false);

  source.addEventListener('error', function(e) {
    if (e.target.readyState != EventSource.OPEN) {
      console.log("Events Disconnected");
    }
  }, false);

  source.addEventListener('message', function(e) {
    console.log("message", e.data);
  }, false);

  source.addEventListener('new_readings', function(e) {
    console.log("new_readings", e.data);
    var myObj = JSON.parse(e.data);
    console.log(myObj);
	angle.value = myObj.sensor1;
    plot(myObj);
  }, false);
}
