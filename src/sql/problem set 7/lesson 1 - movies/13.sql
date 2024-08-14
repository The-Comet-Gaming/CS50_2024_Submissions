SELECT p1.name
FROM people AS p1
JOIN stars AS s1 ON p1.id = s1.person_id
JOIN movies AS m ON s1.movie_id = m.id
WHERE s1.movie_id IN
(
    SELECT s2.movie_id
    FROM stars AS s2
    JOIN people AS p2 ON s2.person_id = p2.id
    WHERE p2.name = 'Kevin Bacon' AND p2.birth = 1958
) AND p1.name != 'Kevin Bacon' AND p1.birth != 1958;