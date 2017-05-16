import { Component, OnInit } from '@angular/core';

import { AlarmService } from './alarm.service';

@Component({
	selector: 'app-root',
	templateUrl: './app.component.html',
	styleUrls: ['./app.component.less']
})
export class AppComponent implements OnInit {
	title = 'app works!';
	alarms = [];

	constructor(private alarmService: AlarmService){}

	ngOnInit() {
		this.alarmService.getAlarms().then((alarms) => {
			this.alarms = alarms;
		});
	}
}
