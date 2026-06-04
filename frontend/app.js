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
    },

    async deleteCategory(categoryId) {
        try {
            const response = await fetch(`${API_BASE_URL}/category/${categoryId}`, {
                method: 'DELETE'
            });
            if (!response.ok) throw new Error('Ошибка при удалении категории');
            return true;
        } catch (error) {
            console.error('API Error (deleteCategory):', error);
            throw error;
        }
    },

    async getResources(userId) {
        try {
            const response = await fetch(`${API_BASE_URL}/user/${userId}/resources`);
            if (!response.ok) throw new Error('Не удалось загрузить ресурсы');
            return await response.json();
        } catch (error) {
            console.error('API Error (getResources):', error);
            return [];
        }
    },

    async createResource(name, path, categoryId, userId) {
        try {
            const response = await fetch(`${API_BASE_URL}/resource`, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({
                    name: name,
                    description: "",
                    path: path,
                    category_id: categoryId,
                    user_id: userId
                })
            });
            if (!response.ok) throw new Error('Ошибка при создании ресурса');
            return await response.json();
        } catch (error) {
            console.error('API Error (createResource):', error);
            throw error;
        }
    },

    async launchResource(resourceId) {
        try {
            const response = await fetch(`${API_BASE_URL}/resource/launch/${resourceId}`, {
                method: 'POST'
            });
            if (!response.ok) throw new Error('Не удалось запустить ресурс');
            return true;
        } catch (error) {
            console.error('API Error (launchResource):', error);
            throw error;
        }
    },

    async deleteResource(resourceId) {
        try {
            const response = await fetch(`${API_BASE_URL}/resource/${resourceId}`, {
                method: 'DELETE'
            });
            if (!response.ok) throw new Error('Ошибка при удалении ресурса');
            return true;
        } catch (error) {
            console.error('API Error (deleteResource):', error);
            throw error;
        }
    }
};

let appState = {
    currentUser: null,       
    categories: [],          
    resources: [],
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
    appState.resources = await apiService.getResources(appState.currentUser.id);

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
        groupWrapper.style.marginBottom = '15px';

        const header = document.createElement('div');
        header.style.display = 'flex';
        header.style.justifyContent = 'space-between';
        header.style.alignItems = 'center';
        header.style.padding = '4px 6px';
        header.style.borderRadius = '4px';
        if (cat.id === appState.activeCategoryId) {
            header.style.backgroundColor = 'rgba(59, 72, 93, 0.3)';
        }

        const titleSpan = document.createElement('span');
        titleSpan.style.fontSize = '13px';
        titleSpan.style.color = cat.id === appState.activeCategoryId ? 'var(--text-main)' : 'var(--text-muted)';
        titleSpan.style.fontWeight = '600';
        titleSpan.style.cursor = 'pointer';
        titleSpan.textContent = cat.name;
        titleSpan.addEventListener('click', () => selectCategory(cat.id));
        header.appendChild(titleSpan);

        const deleteBtn = document.createElement('button');
        deleteBtn.innerHTML = '×';
        deleteBtn.style.background = 'none';
        deleteBtn.style.border = 'none';
        deleteBtn.style.color = 'var(--text-muted)';
        deleteBtn.style.cursor = 'pointer';
        deleteBtn.style.fontSize = '16px';
        deleteBtn.style.padding = '0 4px';
        deleteBtn.title = 'Удалить категорию';
        
        deleteBtn.addEventListener('click', async (e) => {
            e.stopPropagation();
            if (confirm(`Удалить категорию "${cat.name}" и все её ресурсы?`)) {
                await apiService.deleteCategory(cat.id);
                if (appState.activeCategoryId === cat.id) appState.activeCategoryId = null;
                await loadDashboard();
            }
        });
        header.appendChild(deleteBtn);

        groupWrapper.appendChild(header);

        const itemsList = document.createElement('div');
        
        if (cat.id === appState.activeCategoryId) {
            const filtered = appState.resources.filter(r => r.category_id === cat.id);
            filtered.forEach(res => {
                const subItem = document.createElement('div');
                subItem.classList.add('sidebar-item');
                subItem.style.display = 'flex';
                subItem.style.alignItems = 'center';
                subItem.style.justifyContent = 'space-between';
                subItem.style.padding = '6px 10px';
                subItem.style.fontSize = '13px';
                
                const leftContent = document.createElement('div');
                leftContent.style.display = 'flex';
                leftContent.style.alignItems = 'center';
                leftContent.style.gap = '10px';
                leftContent.style.cursor = 'pointer';
                leftContent.innerHTML = `
                    <div style="width: 16px; height: 16px; background: #59616e; border-radius: 3px;"></div>
                    <span>${res.name}</span>
                `;
                leftContent.addEventListener('click', () => apiService.launchResource(res.id));
                subItem.appendChild(leftContent);

                const delResBtn = document.createElement('span');
                delResBtn.innerHTML = '×';
                delResBtn.style.cursor = 'pointer';
                delResBtn.style.color = 'var(--text-muted)';
                delResBtn.addEventListener('click', async (e) => {
                    e.stopPropagation();
                    if (confirm(`Удалить ${res.name}?`)) {
                        await apiService.deleteResource(res.id);
                        await loadDashboard();
                    }
                });
                subItem.appendChild(delResBtn);

                itemsList.appendChild(subItem);
            });
        }

        groupWrapper.appendChild(itemsList);
        sidebarListEl.appendChild(groupWrapper);
    });
}

function renderTabs() {
    tabsContainer.innerHTML = '';
    appState.categories.forEach(cat => {
        const tab = document.createElement('div');
        
        tab.style.display = "flex";
        tab.style.alignItems = "center";
        tab.style.gap = "8px";
        tab.style.padding = "8px 12px";
        tab.style.fontSize = "13px";
        tab.style.cursor = "pointer";
        tab.style.backgroundColor = cat.id === appState.activeCategoryId ? "#28303d" : "#1a1d24";
        tab.style.color = cat.id === appState.activeCategoryId ? "#fff" : "var(--text-muted)";
        tab.style.borderTopLeftRadius = "6px";
        tab.style.borderTopRightRadius = "6px";
        tab.style.border = "1px solid var(--border-color)";
        tab.style.borderBottom = "none";
        tab.style.whiteSpace = "nowrap";

        const tabTitle = document.createElement('span');
        tabTitle.textContent = cat.name;
        tabTitle.addEventListener('click', () => selectCategory(cat.id));
        tab.appendChild(tabTitle);

        const closeTabBtn = document.createElement('span');
        closeTabBtn.innerHTML = '×';
        closeTabBtn.style.color = 'var(--text-muted)';
        closeTabBtn.style.cursor = 'pointer';
        closeTabBtn.style.fontSize = '14px';
        closeTabBtn.addEventListener('click', async (e) => {
            e.stopPropagation();
            if (confirm(`Удалить категорию "${cat.name}"?`)) {
                await apiService.deleteCategory(cat.id);
                if (appState.activeCategoryId === cat.id) appState.activeCategoryId = null;
                await loadDashboard();
            }
        });
        tab.appendChild(closeTabBtn);

        tabsContainer.appendChild(tab);
    });
}

function selectCategory(id) {
    appState.activeCategoryId = id;
    
    renderSidebar();
    renderTabs();
    
    gridContainerEl.innerHTML = '';
    
    const filtered = appState.resources.filter(r => r.category_id === id);
    
    if (filtered.length === 0) {
        showEmptyState();
        return;
    }
    
    showGrid();
    filtered.forEach(res => {
        const card = document.createElement('div');
        card.classList.add('card');
        card.style.position = 'relative';
        
        card.innerHTML = `
            <div class="card-preview"></div>
            <div class="card-title">${res.name}</div>
            <button class="win-btn" class="delete-card-btn" style="position: absolute; top: 8px; right: 8px; width: 24px; height: 24px; display: none; line-height: 20px; font-size: 14px; background: rgba(40,48,61,0.8);">×</button>
        `;

        card.addEventListener('click', () => apiService.launchResource(res.id));
        
        const delBtn = card.querySelector('button');
        card.addEventListener('mouseenter', () => delBtn.style.display = 'flex');
        card.addEventListener('mouseleave', () => delBtn.style.display = 'none');
        
        delBtn.addEventListener('click', async (e) => {
            e.stopPropagation();
            if (confirm(`Удалить ${res.name}?`)) {
                await apiService.deleteResource(res.id);
                await loadDashboard();
            }
        });

        gridContainerEl.appendChild(card);
    });
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

document.getElementById('btn-download').addEventListener('click', async () => {
    if (!appState.currentUser || !appState.activeCategoryId) return alert('Выберите категорию для добавления ресурса!');
    
    const name = prompt('Введите название программы/ссылки:');
    if (!name) return;
    
    const path = prompt('Введите путь к файлу или URL ссылку:');
    if (!path) return;

    try {
        await apiService.createResource(name, path, appState.activeCategoryId, appState.currentUser.id);
        await loadDashboard();
    } catch (error) {
        alert('Не удалось добавить ресурс');
    }
});
