SELECT P.lastName, count(*)
FROM Persons AS P
WHERE P.lastName IN
	SELECT P.lastName
	FROM Persons AS P
	WHERE P.age > 45