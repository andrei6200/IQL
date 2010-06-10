	SELECT P.firstName, P.lastName
	FROM Persons AS P
EXCEPT
	SELECT R.firstName, R.lastName
	FROM Ratings AS R