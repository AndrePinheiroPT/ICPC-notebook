/**
 * Author: 
 * Date: 
 * License: CC0
 * Source: folklore
 * Description: Classic binary search algorithm
 * Status: tested
 */


int l = 0, r = n+1; // check(l) = true | check(r) = false
while(l+1<r){
    int mid = (l+r)/2;
    if(check(mid)) l = mid;
    else r = mid;
}