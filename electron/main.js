const { app, BrowserWindow } = require('electron');
const path = require('path');

function createWindow() {
  const win = new BrowserWindow({
    width: 1000,
    height: 800,
    webPreferences: {
      nodeIntegration: true,
      contextIsolation: false,
    }
  });

  // Загружаем твой интерфейс. 
  // Убедись, что путь правильный относительно main.js!
  // Если index.html лежит в папке renderer:
  win.loadFile(path.join(__dirname, 'renderer/index.html'));

  win.webContents.openDevTools();
}

app.whenReady().then(createWindow);

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') app.quit();
});