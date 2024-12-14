import express from  'express';
import numeral from 'numeral';
import { engine } from 'express-handlebars';
import hbs_sections from 'express-handlebars-sections';

const app = express();

app.use(express.urlencoded({
    extended: true,
}));

// test more
app.engine('hbs', engine({
    extname: 'hbs',
    defaultLayout: 'main',
    helpers: {
        section: hbs_sections(),
    }
}));
app.set('view engine', 'hbs');
app.set('views', './views');

app.use('/public',express.static('public'));

app.get('/', async function(req, res) {
    res.render('home');
})

app.listen(3000, function() {
    console.log('app is running at http://localhost:3000');
})


