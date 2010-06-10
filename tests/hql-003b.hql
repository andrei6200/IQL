SELECT P.firstName, P.lastName, 
	Coll [L.lower1 : L.upper1, 200:L.upper2]
FROM (Persons AS P NATURAL JOIN IntegerLimits AS L),
	PhotoCollection AS Coll
