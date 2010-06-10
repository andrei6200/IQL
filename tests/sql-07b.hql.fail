SELECT P.lastName, count(*)
FROM 
	(SELECT R.lastName, R.firstName
	FROM Ratings AS R
	WHERE R.reputation > 3) 
	AS P
GROUP BY lastName