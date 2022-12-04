
const express = require('express');
const cors = require('cors');
const routers = require('./Routes/Routes.js');
const mediaRoutes = require('./Routes/Media');
const path = require('path');
const port = 3001;

const app=express();

//restfull 
app.use(cors());
app.use(express.json());
app.use(express.urlencoded({ extended: true }));

// making the needed connetions an urls for the API
app.use('/', routers);
app.use('/add_new_media', express.static(path.join(__dirname, '../Client/html/add_new_media.html')));
app.use('/edit_media/:id', express.static(path.join(__dirname, '../Client/html/edit_media.html')));
app.use('/:id/add_new_actor', express.static(path.join(__dirname, '../Client/html/add_new_actor.html')));
app.use('/:id/actors_list', express.static(path.join(__dirname, '../Client/html/actors_list.html')));
app.use('/',express.static(path.join(__dirname, '../Client/html')));
app.use('/js', express.static(path.join(__dirname, '../Client/js')));
app.use('/css', express.static(path.join(__dirname, '../Client/css')));

const server = app.listen(port, () => {
    console.log('listening on port %s...', server.address().port);
});