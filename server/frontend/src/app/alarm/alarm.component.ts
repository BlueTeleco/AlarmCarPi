import { Component, Input } from '@angular/core';

import { AlarmService } from '../alarm.service';

@Component({
	selector: 'alarm',
	templateUrl: './alarm.component.html',
	styleUrls: ['./alarm.component.less']
})
export class AlarmComponent {

	_newAlarm: boolean;

	@Input() alarm: any = {
		time: {
			minute: "00",
			hour: "00"
		},
		repetitions: []
	};

	@Input()
	set newAlarm(value: any) {
		this._newAlarm = value !== 'false';
	};

	constructor(private alarmService: AlarmService) {}

	set time(time: string) {
		let times = time.split(':');
		this.alarm.time.hour = times[0];
		this.alarm.time.minute = times[1];
	}

	get time() {
		return (this.alarm.time.hour.length < 2 ? '0' + this.alarm.time.hour : this.alarm.time.hour) + ':' + (this.alarm.time.minute.length < 2 ? '0' + this.alarm.time.minute : this.alarm.time.minute);
	}

	get repetitions() {
		let repetitions = Array(7).fill(false);
		for (let day of this.alarm.repetitions) {
			repetitions[day] = true;
		}
		return repetitions;
	}

	updateRepetitions(repetition: boolean) {
		this.alarm.repetitions[event.target.id] = repetition;
	}

	saveAlarm(){
		this.alarmService.saveAlarm(this.alarm);
	}

	removeAlarm(){
		this.alarmService.removeAlarm(this.alarm.id);
	}
}
