SELECT title
FROM movies AS m
JOIN stars AS s ON m.id = s.movie_id
JOIN ratings AS r ON m.id = r.movie_id
JOIN people AS p ON s.person_id = p.id
WHERE name ='Chadwick Boseman'
ORDER BY r.rating DESC
LIMIT 5;
