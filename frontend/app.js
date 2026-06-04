const API_BASE_URL = 'http://localhost:18080';

const apiService = {
    async getUsers() {
        try {
            const response = await fetch(`${API_BASE_URL}/users`);
            if (!response.ok) throw new Error('Не удалось загрузить пользователей');
            return await response.json();
        } catch (error) {
            console.error('API Error (getUsers):', error);
            return [];
        }
    },

    async loginUser(name) { 
        try {
            const response = await fetch(`${API_BASE_URL}/user`, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ name: name, avatar_url: "" })
            });
            if (!response.ok) throw new Error('Ошибка при логине');
            return await response.json();
        } catch (error) {
            console.error('API Error (loginUser):', error);
            throw error;
        }
    },

    async deleteUser(userId) {
        try {
            const response = await fetch(`${API_BASE_URL}/user/${userId}`, {
                method: 'DELETE'
            });
            if (!response.ok) throw new Error('Не удалось удалить пользователя');
            return true;
        } catch (error) {
            console.error('API Error (deleteUser):', error);
            throw error;
        }
    },

    async getCategories(userId) {
        try {
            const response = await fetch(`${API_BASE_URL}/user/${userId}/categories`);
            if (!response.ok) throw new Error('Не удалось загрузить категории');
            return await response.json(); 
        } catch (error) {
            console.error('API Error (getCategories):', error);
            return [];
        }
    },

    async createCategory(name, userId) {
        try {
            const response = await fetch(`${API_BASE_URL}/category`, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({
                    name: name,
                    description: "Создано из Universal Manager",
                    user_id: userId
                })
            });
            if (!response.ok) throw new Error('Ошибка при создании категории');
            return await response.json();
        } catch (error) {
            console.error('API Error (createCategory):', error);
            throw error;
        }
    }
};

let appState = {
    currentUser: null,       
    categories: [],          
    activeCategoryId: null   
};

const loginOverlay = document.getElementById('login-overlay');
const loginForm = document.getElementById('login-form');
const userControlsContainer = document.getElementById('user-controls-container');
const userSelectEl = document.getElementById('user-select');
const btnAddUser = document.getElementById('btn-add-user');
const btnDeleteUser = document.getElementById('btn-delete-user');
const sidebarListEl = document.getElementById('sidebar-collections-list');
const emptyStateEl = document.getElementById('empty-state');
const gridContainerEl = document.getElementById('grid-container');
const tabsContainer = document.getElementById('tabs-container');

loginForm.addEventListener('submit', async (e) => {
    e.preventDefault();
    const nameInput = document.getElementById('login-name');
    const name = nameInput.value.trim();

    try {
        const currentUsers = await apiService.getUsers();
        const userExists = currentUsers.some(u => u.name.toLowerCase() === name.toLowerCase());

        if (!userExists) {
            const confirmCreation = confirm(`Пользователь "${name}" не найден. Создать новый профиль?`);
            if (!confirmCreation) return;
        }

        const userData = await apiService.loginUser(name);
        await setCurrentUser(userData);
        loginOverlay.style.display = 'none';
        nameInput.value = '';
    } catch (err) {
        alert('Не удалось подключиться к бэкенду. Проверь, запущен ли сервер.');
    }
});

userSelectEl.addEventListener('change', async (e) => {
    const selectedId = e.target.value;
    const selectedName = e.target.options[e.target.selectedIndex].text;
    
    appState.activeCategoryId = null; 
    await setCurrentUser({ id: parseInt(selectedId), name: selectedName });
});

btnAddUser.addEventListener('click', () => {
    loginOverlay.style.display = 'flex';
    document.getElementById('login-name').focus();
});

btnDeleteUser.addEventListener('click', async () => {
    if (!appState.currentUser || !appState.currentUser.id) return;

    const confirmDelete = confirm(`Вы уверены, что хотите удалить профиль "${appState.currentUser.name}"?`);
    if (!confirmDelete) return;

    try {
        await apiService.deleteUser(appState.currentUser.id);

        const remainingUsers = await apiService.getUsers();
        if (remainingUsers.length > 0) {
            await setCurrentUser(remainingUsers[0]);
        } else {
            appState.currentUser = null;
            userControlsContainer.style.display = 'none';
            loginOverlay.style.display = 'flex';
        }
    } catch (err) {
        alert('Ошибка при удалении пользователя.');
    }
});

async function setCurrentUser(user) {
    appState.currentUser = user;
    userControlsContainer.style.display = 'flex';
    
    const allUsers = await apiService.getUsers();
    
    userSelectEl.innerHTML = '';
    allUsers.forEach(u => {
        const opt = document.createElement('option');
        opt.value = u.id;
        opt.textContent = u.name;
        if (u.id === user.id) opt.selected = true;
        userSelectEl.appendChild(opt);
    });

    await loadDashboard();
}

async function loadDashboard() {
    if (!appState.currentUser || !appState.currentUser.id) return;

    appState.categories = await apiService.getCategories(appState.currentUser.id);

    if (!appState.categories || appState.categories.length === 0) {
        showEmptyState();
        sidebarListEl.innerHTML = '';
        tabsContainer.innerHTML = '';
        return;
    }

    showGrid();
    renderSidebar();
    renderTabs();

    if (!appState.activeCategoryId && appState.categories.length > 0) {
        selectCategory(appState.categories[0].id);
    } else if (appState.activeCategoryId) {
        selectCategory(appState.activeCategoryId);
    }
}

function showEmptyState() {
    emptyStateEl.style.display = 'block';
    gridContainerEl.style.display = 'none';
}

function showGrid() {
    emptyStateEl.style.display = 'none';
    gridContainerEl.style.display = 'grid';
}

function renderSidebar() {
    sidebarListEl.innerHTML = '';
    
    appState.categories.forEach(cat => {
        const groupWrapper = document.createElement('div');
        groupWrapper.style.marginBottom = '20px';

        const header = document.createElement('div');
        header.style.fontSize = '12px';
        header.style.color = 'var(--text-muted)';
        header.style.fontWeight = '600';
        header.style.marginBottom = '8px';
        header.style.cursor = 'pointer';
        header.textContent = `${cat.name} (5)`;
        
        header.addEventListener('click', () => selectCategory(cat.id));
        groupWrapper.appendChild(header);

        const itemsList = document.createElement('div');
        
        if (cat.id === appState.activeCategoryId) {
            for(let i = 0; i < 5; i++) {
                const subItem = document.createElement('div');
                subItem.classList.add('sidebar-item');
                subItem.style.display = 'flex';
                subItem.style.alignItems = 'center';
                subItem.style.gap = '10px';
                subItem.style.padding = '6px 10px';
                subItem.style.fontSize = '13px';
                
                subItem.innerHTML = `
                    <div style="width: 16px; height: 16px; background: #59616e; border-radius: 3px;"></div>
                    <span>Name Entiti</span>
                `;
                itemsList.appendChild(subItem);
            }
        }
        
        groupWrapper.appendChild(itemsList);
        sidebarListEl.appendChild(groupWrapper);
    });
}

function renderTabs() {
    tabsContainer.innerHTML = '';
    appState.categories.forEach(cat => {
        const tab = document.createElement('div');
        
        tab.style.padding = "8px 16px";
        tab.style.fontSize = "13px";
        tab.style.cursor = "pointer";
        tab.style.backgroundColor = cat.id === appState.activeCategoryId ? "#28303d" : "#1a1d24";
        tab.style.color = cat.id === appState.activeCategoryId ? "#fff" : "var(--text-muted)";
        tab.style.borderTopLeftRadius = "6px";
        tab.style.borderTopRightRadius = "6px";
        tab.style.border = "1px solid var(--border-color)";
        tab.style.borderBottom = "none";
        tab.style.whiteSpace = "nowrap";

        tab.textContent = `${cat.name} (5)`;
        tab.addEventListener('click', () => selectCategory(cat.id));
        tabsContainer.appendChild(tab);
    });
}

function selectCategory(id) {
    appState.activeCategoryId = id;
    renderSidebar();
    renderTabs();
    
    gridContainerEl.innerHTML = '';
    
    for (let i = 0; i < 5; i++) {
        const card = document.createElement('div');
        card.classList.add('card');
        card.innerHTML = `
            <div class="card-preview"></div>
            <div class="card-title">Name Entiti</div>
        `;
        gridContainerEl.appendChild(card);
    }
}

document.getElementById('btn-add-collection').addEventListener('click', async () => {
    if (!appState.currentUser) return alert('Сначала авторизуйтесь!');
    
    const name = prompt('Введите название новой категории:');
    if (!name) return;

    try {
        await apiService.createCategory(name, appState.currentUser.id);
        await loadDashboard();
    } catch (error) {
        alert('Не удалось создать категорию');
    }
});
