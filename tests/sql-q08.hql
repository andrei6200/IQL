SELECT P.firstName, P.lastName, R.rating
FROM (Persons AS P) JOIN (Ratings AS R) 
	ON P.lastName = R.lastName AND P.firstName != R.lastName