import fs from 'fs';

export default {
    docDuLieu(filePath) {
        return new Promise((resolve, reject) => {
            fs.readFile(filePath, 'utf8', (err, data) => {
                if (err) {
                    return reject('Không thể mở file: ' + err);
                }

                try {
                    const danhSachTram = JSON.parse(data);
                    resolve(danhSachTram);
                } catch (e) {
                    reject('Lỗi khi phân tích file JSON: ' + e);
                }
            });
        });
    }

}

