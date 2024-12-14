import express from 'express';
import { engine } from 'express-handlebars';
import hbs_sections from 'express-handlebars-sections';
import ds_tramService from './services/ds_tram.service.js';

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
        ifEquals(arg1, arg2, options) {
            // console.log(arg1);
            // console.log(arg2);
            return arg1 == arg2 ? options.fn(this) : options.inverse(this);
        },
    }
}));
app.set('view engine', 'hbs');
app.set('views', './views');

app.use('/public', express.static('public'));


app.get('/', async function (req, res) {
    const filepath = "./ds_tram.json";
    const ds_tram = await ds_tramService.docDuLieu(filepath);

    // console.log(ds_tram);
    res.render('home', {
        ds_tram: ds_tram,
    });
})

import { exec, spawn } from 'child_process';
app.post('/find', async function (req, res) {
    const filepath = "./ds_tram.json";
    const ds_tram = await ds_tramService.docDuLieu(filepath);
    const entity = {
        id_start: +req.body.id_start || 0,
        id_end: +req.body.id_end || 0,
    };

    console.log(req.body);
    console.log(entity);

    // Gọi chương trình C++
    const testProcess = spawn('./test');

    // Gửi dữ liệu qua stdin
    testProcess.stdin.write(`${entity.id_start} ${entity.id_end}\n`);
    testProcess.stdin.end();

    let result = "";
    testProcess.stdout.on('data', (data) => {
        result += data.toString();
    });

    testProcess.on('close', (code) => {
        // Chuyển đổi chuỗi JSON thành đối tượng
        const jsonObject = JSON.parse(result.trim());

        // Xử lý dữ liệu JSON
        console.log(jsonObject);

        // Nếu bạn cần truy cập vào các câu trả lời
        const answers = jsonObject.answers;
        answers.sort((a, b) => a.length - b.length);

        answers.forEach((answer, index) => {
            console.log(`\n--- Câu trả lời ${index + 1} ---`);
            answer.forEach(tram => {
                console.log(`Trạm: ${tram.tram}`);
                if (tram.xe_buyt) {
                    console.log(`Xe buýt: ${tram.xe_buyt}`);
                    console.log(`Nơi đến: ${tram.noi_den}`);
                }
            });
        });

        res.render('home', {
            ds_tram: ds_tram,
            id_start: entity.id_start,
            id_end: entity.id_end,
            result: result.trim(),
            answers: answers
        });
    });
});

app.listen(3000, function () {
    console.log('app is running at http://localhost:3000');
})


