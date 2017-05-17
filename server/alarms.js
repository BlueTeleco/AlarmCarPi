var crontab = require('crontab');
var alarms = {};
let idIndex = 0;

alarms.init = function () {
	return new Promise((resolve, reject) => {
		crontab.load((err, newCrontab) => {
			if (err) {
				return reject(err);
			}
			crontab = newCrontab;
			return resolve();
		});
	});
}

function mapAlarm(alarm) {
	return {
		id: alarm.comment().toString(),
		time: {
			hour: alarm.hour().toString(),
			minute: alarm.minute().toString()
		},
		repetitions: alarm.dow().toString().split(',')
	};
}

function getId() {
	let id = idIndex;
	let jobs = crontab.jobs().map(mapAlarm).sort(function(a, b){
		return a.id - b.id;
	});
	for (let job of jobs) {
		if(job.id != id){
			return id.toString();
		}
		id++;
	}
	return id.toString();
}

alarms.getAlarms = function () {
	return new Promise(function(resolve, reject) {
		resolve(crontab.jobs().map(mapAlarm));
	});
}

alarms.addAlarm = function(time, repetitions) {
	let date = time.minute + ' ' + time.hour + ' * * ' + repetitions.join(',');
	crontab.create('/home/pi/SDG2/controller/alarma', date, getId());
	return new Promise(function(resolve, reject) {
		crontab.save((err, crontab) => {
			if (err) {
				return reject(err);
			}
			return resolve();
		});
	});

}

alarms.removeAlarm = function (alarmId) {
	crontab.remove({comment:alarmId.toString()});
	return new Promise(function(resolve, reject) {
		crontab.save((err, crontab) => {
			if (err) {
				return reject(err);
			}
			return resolve();
		});
	});
}

module.exports = alarms;