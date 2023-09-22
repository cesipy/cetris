# TODO

- [ ] add other pieces than just I
- [ ] fix error of weird block at first position
- [ ] make whole game structure simpler
- [ ] add score
- [ ] make size appropriate (height)
- [ ] fix bug on left edge
- [ ] can rotate function
- [ ] when game-over -> screen showing  status 
- [x] check if rotation is allowed
- [x] better middle point for L and J shapes. not at the edge, 
but in the middle
- [ ] remove red middle point
- [x] eliminate full line
- [ ] fix-17 in manage_full_lines function; problem wit game->cols
- [ ] 8 (color value for empty) as makro
- [ ] adjust_blocks: fix condition to ask if i+1 block is free
- [ ] refactor WIN* (game->win, instead of *board, *score, …)
## add rotation:
specify a middle point for each falling piece. 
there is always only one falling piece, so it can 
be stored in game struct. like a vector field rotation, 
all four fields around the middle can be rotated