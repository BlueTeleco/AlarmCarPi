var express = require('express'),
bodyParser = require("body-parser"),
app = express(),
alarms = require('./alarms');

alarms.init().then(()=>{

	app.use(bodyParser.json({ limit: '50mb' }));
	app.use('/api', require('./api'))

	app.use('/', express.static(__dirname + '/frontend'));

	app.listen(3001, function () {
		console.log('Example app listening on port 3001!');
	});
})
.catch((err)=>{
	console.error(err.message);
	process.exit(-1);
});

/*app.get('/', function (req, res) {
res.send('Hello World!');
});*/




