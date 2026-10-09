/**
 * Author: jet notebook
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item gregorianDays(y,m,d) returns days since 1970-01-01 (zero).
 *  Input is a valid date; months/days start at 1, unlike array indices.
 *  Supports year 0 and negative years; $-10^{12}\le y\le10^{12}$.
 *  \item Subtract results for elapsed days; add 1 for inclusive counting.
 *  Proleptic Gregorian calendar: ignores historical calendar changes.
 *  Weekday Monday=0: ((days+3)\%7+7)\%7. $O(1)$ time/space.
 *  \end{itemize}
 * Usage: ll days=gregorianDays(2024,3,1)-gregorianDays(2024,2,28); // 2
 */
#pragma once

ll gregorianDays(ll y, int m, int d) {
	y -= m <= 2;
	ll era = y/400, z = y-era*400;
	int day = (153*(m+(m>2 ? -3 : 9))+2)/5+d-1;
	ll off = z*365+z/4-z/100+day;
	return era*146097+off-719468;
}