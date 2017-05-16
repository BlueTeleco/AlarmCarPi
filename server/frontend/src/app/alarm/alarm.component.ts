import { Component, Input } from '@angular/core';

@Component({
	selector: 'alarm',
	templateUrl: './alarm.component.html',
	styleUrls: ['./alarm.component.less']
})
export class AlarmComponent {

	@Input() alarm: any;

	constructor() { }

}
