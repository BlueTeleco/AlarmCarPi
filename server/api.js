const express = require('express'),
alarms = require('./alarms');

let router = express.Router();

router.route('/alarm').get((req, res, next) => {
	alarms.getAlarms().then((alarms)=>{
		return res.status(200).send(alarms);
	})
	.catch((err)=>{
		return res.status(400).send(err);
	});
});
router.route('/alarm').post((req, res, next) => {
	let tasks = [];
	if(req.body.id){
		tasks.push(alarms.removeAlarm(req.body.id));
	}
	tasks.push(alarms.addAlarm(req.body.time, req.body.repetitions));
	Promise.all(tasks).then(()=>{
		return res.status(200).send(true);
	}).catch((err)=>{
		return res.status(400).send(err);
	});

});
router.route('/alarm/:id').delete((req, res, next) => {
	alarms.removeAlarm(req.params.id).then(()=>{
		return res.status(200).send(true);
	})
	.catch((err)=>{
		return res.status(400).send(err);
	});
});
module.exports = router;