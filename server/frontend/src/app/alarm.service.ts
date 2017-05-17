import { Injectable, OnInit } from '@angular/core';
import { Headers, Http, Response, RequestOptions } from '@angular/http';

import 'rxjs/add/operator/toPromise';

@Injectable()
export class AlarmService {

	constructor(private http: Http) {}

	getAlarms(): Promise<any> {
		return this.http.get('api/alarm')
		.toPromise()
		.then((response: Response) => {
			return response.json();
		});
	}

	saveAlarm(alarm: any): Promise<any> {
		return this.http.post('api/alarm', alarm)
		.toPromise()
		.then((response: Response) => {
			return response.json();
		});
	}

	removeAlarm(id: any): Promise<any> {
		return this.http.delete('api/alarm/' + id)
		.toPromise()
		.then((response: Response) => {
			return response.json();
		});
	}
}
