const API_BASE_URL = 'http://localhost:18080';

const apiService = {
    async getUsers() {
        try {
            const response = await fetch(`${API_BASE_URL}/users`);
            if (!response.ok) throw new Error('Failed to fetch users');
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
            if (!response.ok) throw new Error('Login failed');
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
            if (!response.ok) throw new Error('Delete user failed');
            return true;
        } catch (error) {
            console.error('API Error (deleteUser):', error);
            throw error;
        }
    },

    async getCategories(userId) {
        try {
            const response = await fetch(`${API_BASE_URL}/user/${userId}/categories`);
            if (!response.ok) throw new Error('Failed to fetch categories');
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
                    description: "Created from Universal Manager",
                    user_id: userId
                })
            });
            if (!response.ok) throw new Error('Create category failed');
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
            if (!response.ok) throw new Error('Delete category failed');
            return true;
        } catch (error) {
            console.error('API Error (deleteCategory):', error);
            throw error;
        }
    },

    async getResources(userId) {
        try {
            const response = await fetch(`${API_BASE_URL}/user/${userId}/resources`);
            if (!response.ok) throw new Error('Failed to fetch resources');
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
            if (!response.ok) throw new Error('Create resource failed');
            return await response.json();
        } catch (error) {
            console.error('API Error (createResource):', error);
            throw error;
        }
    },

    async updateResource(resourceId, name, path, description, avatarUrl, categoryId, userId) {
        try {
            const response = await fetch(`${API_BASE_URL}/resource/${resourceId}`, {
                method: 'PUT',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({
                    name: name,
                    description: description,
                    path: path,
                    avatar_url: avatarUrl,
                    category_id: categoryId,
                    user_id: userId
                })
            });
            if (!response.ok) throw new Error('Update resource failed');
            return await response.json();
        } catch (error) {
            console.error('API Error (updateResource):', error);
            throw error;
        }
    },

    async launchResource(resourceId) {
        try {
            const response = await fetch(`${API_BASE_URL}/resource/launch/${resourceId}`, {
                method: 'POST'
            });
            if (!response.ok) throw new Error('Launch resource failed');
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
            if (!response.ok) throw new Error('Delete resource failed');
            return true;
        } catch (error) {
            console.error('API Error (deleteResource):', error);
            throw error;
        }
    },

    async getPresets(userId) {
        try {
            const response = await fetch(`${API_BASE_URL}/user/${userId}/presets`);
            if (!response.ok) throw new Error('Failed to fetch presets');
            return await response.json();
        } catch (error) {
            console.error('API Error (getPresets):', error);
            return [];
        }
    },

    async createPreset(name, userId) {
        try {
            const response = await fetch(`${API_BASE_URL}/presets`, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ name: name, description: "Bulk launch bundle", user_id: userId })
            });
            if (!response.ok) throw new Error('Create preset failed');
            return await response.json();
        } catch (error) {
            console.error('API Error (createPreset):', error);
            throw error;
        }
    },

    async deletePreset(presetId) {
        try {
            const response = await fetch(`${API_BASE_URL}/presets/${presetId}`, { method: 'DELETE' });
            if (!response.ok) throw new Error('Delete preset failed');
            return true;
        } catch (error) {
            console.error('API Error (deletePreset):', error);
            throw error;
        }
    },

    async addResourceToPreset(presetId, resourceId) {
        try {
            const response = await fetch(`${API_BASE_URL}/presets/${presetId}/resources/${resourceId}`, { method: 'POST' });
            if (!response.ok) throw new Error('Add resource to preset failed');
            return true;
        } catch (error) {
            console.error('API Error (addResourceToPreset):', error);
            throw error;
        }
    },

    async removeResourceFromPreset(presetId, resourceId) {
        try {
            const response = await fetch(`${API_BASE_URL}/presets/${presetId}/resources/${resourceId}`, { method: 'DELETE' });
            if (!response.ok) throw new Error('Remove resource from preset failed');
            return true;
        } catch (error) {
            console.error('API Error (removeResourceFromPreset):', error);
            throw error;
        }
    },

    async launchPreset(presetId) {
        try {
            const response = await fetch(`${API_BASE_URL}/presets/run/${presetId}`, { method: 'POST' });
            if (!response.ok) throw new Error('Launch preset failed');
            return true;
        } catch (error) {
            console.error('API Error (launchPreset):', error);
            throw error;
        }
    }
};

let appState = {
    currentUser: null,       
    categories: [],          
    resources: [],
    presets: [],
    activeCategoryId: null,
    activePresetId: null,
    selectedResource: null,
    temporaryBase64Image: ""
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
const contentBodyEl = document.getElementById('content-body');

const detailArea = document.getElementById('detail-area');
const inputDetailName = document.getElementById('input-detail-name');
const inputDetailPath = document.getElementById('input-detail-path');
const inputDetailDescription = document.getElementById('input-detail-description');
const btnDetailDelete = document.getElementById('btn-detail-delete');
const btnDetailRun = document.getElementById('btn-detail-run');
const btnDetailSave = document.getElementById('btn-detail-save');
const btnDetailBack = document.getElementById('btn-detail-back');

const btnChangeImage = document.getElementById('btn-change-image');
const detailImgRender = document.getElementById('detail-img-render');
const detailPlaceholderSvg = document.getElementById('detail-placeholder-svg');

if (loginForm) {
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
            if (loginOverlay) loginOverlay.style.display = 'none';
            nameInput.value = '';
        } catch (err) {
            alert('Не удалось подключиться к бэкенду. Проверь, запущен ли сервер.');
        }
    });
}

if (userSelectEl) {
    userSelectEl.addEventListener('change', async (e) => {
        const selectedId = e.target.value;
        const selectedName = e.target.options[e.target.selectedIndex].text;
        appState.activeCategoryId = null; 
        appState.activePresetId = null;
        closeDetailArea();
        await setCurrentUser({ id: parseInt(selectedId), name: selectedName });
    });
}

if (btnAddUser) {
    btnAddUser.addEventListener('click', () => {
        if (loginOverlay) loginOverlay.style.display = 'flex';
        const loginNameEl = document.getElementById('login-name');
        if (loginNameEl) loginNameEl.focus();
    });
}

if (btnDeleteUser) {
    btnDeleteUser.addEventListener('click', async () => {
        if (!appState.currentUser || !appState.currentUser.id) return;
        if (!confirm(`Delete profile "${appState.currentUser.name}"?`)) return;

        try {
            await apiService.deleteUser(appState.currentUser.id);
            const remainingUsers = await apiService.getUsers();
            closeDetailArea();
            if (remainingUsers.length > 0) {
                await setCurrentUser(remainingUsers[0]);
            } else {
                appState.currentUser = null;
                if (userControlsContainer) userControlsContainer.style.display = 'none';
                if (loginOverlay) loginOverlay.style.display = 'flex';
            }
        } catch (err) {
            alert('Error deleting user');
        }
    });
}

if (btnDetailBack) {
    btnDetailBack.addEventListener('click', () => {
        closeDetailArea();
    });
}

async function setCurrentUser(user) {
    appState.currentUser = user;
    if (userControlsContainer) userControlsContainer.style.display = 'flex';
    
    const allUsers = await apiService.getUsers();
    if (userSelectEl) {
        userSelectEl.innerHTML = '';
        allUsers.forEach(u => {
            const opt = document.createElement('option');
            opt.value = u.id;
            opt.textContent = u.name;
            if (u.id === user.id) opt.selected = true;
            userSelectEl.appendChild(opt);
        });
    }

    await loadDashboard();
}

async function loadDashboard() {
    if (!appState.currentUser || !appState.currentUser.id) return;

    appState.categories = await apiService.getCategories(appState.currentUser.id);
    appState.resources = await apiService.getResources(appState.currentUser.id);

    const serverPresets = await apiService.getPresets(appState.currentUser.id);
    appState.presets = serverPresets.map(sp => {
        const localPreset = appState.presets.find(lp => lp.id === sp.id);
        if (localPreset && (!sp.resource_ids || sp.resource_ids.length === 0)) {
            sp.resource_ids = localPreset.resource_ids;
        }
        if (!sp.resource_ids) sp.resource_ids = [];
        return sp;
    });

    if ((!appState.categories || appState.categories.length === 0) && (!appState.presets || appState.presets.length === 0)) {
        showEmptyState();
        sidebarListEl.innerHTML = '';
        tabsContainer.innerHTML = '';
        return;
    }

    showGrid();
    renderSidebar();
    renderTabs();

    if (!appState.activeCategoryId && !appState.activePresetId && appState.categories.length > 0) {
        selectCategory(appState.categories[0].id);
    } else if (appState.activeCategoryId) {
        selectCategory(appState.activeCategoryId);
    } else if (appState.activePresetId) {
        selectPreset(appState.activePresetId);
    }
}

function showEmptyState() {
    emptyStateEl.style.display = 'block';
    gridContainerEl.style.display = 'none';
    closeDetailArea();
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
        
        deleteBtn.addEventListener('click', async (e) => {
            e.stopPropagation();
            if (confirm(`Delete category "${cat.name}"?`)) {
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

                let avatarStyle = `background: #59616e; border-radius: 3px;`;
                if (res.avatar_url) {
                    const imgSrc = res.avatar_url.startsWith('/home/') 
                        ? `${API_BASE_URL}/host-media?path=${encodeURIComponent(res.avatar_url)}`
                        : res.avatar_url;
                    avatarStyle = `background-image: url('${imgSrc}'); background-size: cover; background-position: center; border-radius: 50%;`;
                }

                leftContent.innerHTML = `
                    <div style="width: 18px; height: 18px; flex-shrink: 0; ${avatarStyle}"></div>
                    <span>${res.name}</span>
                `;
                leftContent.addEventListener('click', () => showResourceDetails(res));
                subItem.appendChild(leftContent);

                const delResBtn = document.createElement('span');
                delResBtn.innerHTML = '×';
                delResBtn.style.cursor = 'pointer';
                delResBtn.style.color = 'var(--text-muted)';
                delResBtn.addEventListener('click', async (e) => {
                    e.stopPropagation();
                    if (confirm(`Delete ${res.name}?`)) {
                        await apiService.deleteResource(res.id);
                        if (appState.selectedResource && appState.selectedResource.id === res.id) closeDetailArea();
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

    const presetSectionWrapper = document.createElement('div');
    presetSectionWrapper.style.marginTop = '25px';
    presetSectionWrapper.style.borderTop = '1px solid #343d4c';
    presetSectionWrapper.style.paddingTop = '15px';

    const presetHeader = document.createElement('div');
    presetHeader.style.display = 'flex';
    presetHeader.style.justifyContent = 'space-between';
    presetHeader.style.alignItems = 'center';
    presetHeader.style.marginBottom = '10px';
    presetHeader.innerHTML = `
        <span style="font-size: 11px; font-weight: 700; color: var(--text-muted); letter-spacing: 1px;">⚡ ПРЕСЕТЫ СВЯЗОК</span>
        <button id="btn-add-preset" style="background:none; border:none; color:var(--text-muted); cursor:pointer; font-size:16px; padding:0 5px; font-weight: bold;">+</button>
    `;
    presetSectionWrapper.appendChild(presetHeader);

    appState.presets.forEach(preset => {
        const pItem = document.createElement('div');
        pItem.style.display = 'flex';
        pItem.style.justifyContent = 'space-between';
        pItem.style.alignItems = 'center';
        pItem.style.padding = '8px 10px';
        pItem.style.borderRadius = '6px';
        pItem.style.cursor = 'pointer';
        pItem.style.marginBottom = '4px';
        pItem.style.fontSize = '13px';
        
        if (preset.id === appState.activePresetId) {
            pItem.style.backgroundColor = 'rgba(59, 72, 93, 0.4)';
            pItem.style.color = 'var(--text-main)';
        } else {
            pItem.style.color = 'var(--text-muted)';
        }

        const nameSpan = document.createElement('span');
        nameSpan.textContent = `🚀 ${preset.name}`;
        nameSpan.addEventListener('click', () => selectPreset(preset.id));
        pItem.appendChild(nameSpan);

        const delBtn = document.createElement('span');
        delBtn.innerHTML = '×';
        delBtn.style.cursor = 'pointer';
        delBtn.style.fontSize = '16px';
        delBtn.style.color = 'var(--text-muted)';
        delBtn.addEventListener('click', async (e) => {
            e.stopPropagation();
            if (confirm(`Удалить пресет связки "${preset.name}"?`)) {
                await apiService.deletePreset(preset.id);
                if (appState.activePresetId === preset.id) appState.activePresetId = null;
                await loadDashboard();
            }
        });
        pItem.appendChild(delBtn);
        presetSectionWrapper.appendChild(pItem);
    });

    sidebarListEl.appendChild(presetSectionWrapper);

    const btnAddPreset = document.getElementById('btn-add-preset');
    if (btnAddPreset) {
        btnAddPreset.onclick = async () => {
            const name = prompt('Введите имя нового пакетного пресета:');
            if (!name) return;
            try {
                await apiService.createPreset(name, appState.currentUser.id);
                await loadDashboard();
            } catch (err) {
                alert('Не удалось добавить пресет.');
            }
        };
    }
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
            if (confirm(`Delete category "${cat.name}"?`)) {
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
    appState.activePresetId = null; 
    renderSidebar();
    renderTabs();
    closeDetailArea();
    
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
        
        if (res.avatar_url) {
            const imgSrc = res.avatar_url.startsWith('/home/') 
                ? `${API_BASE_URL}/host-media?path=${encodeURIComponent(res.avatar_url)}`
                : res.avatar_url;

            card.innerHTML = `
                <div class="card-preview" style="background: url('${imgSrc}') center/cover no-repeat; border-radius: 8px;"></div>
                <div class="card-title">${res.name}</div>
            `;
        } else {
            card.innerHTML = `
                <div class="card-preview"></div>
                <div class="card-title">${res.name}</div>
            `;
        }
        
        card.addEventListener('click', () => showResourceDetails(res));
        gridContainerEl.appendChild(card);
    });
}

function selectPreset(presetId) {
    appState.activePresetId = presetId;
    appState.activeCategoryId = null; 
    
    renderSidebar();
    renderTabs();
    closeDetailArea();
    
    const currentPreset = appState.presets.find(p => p.id === presetId);
    if (!currentPreset) return;

    gridContainerEl.innerHTML = '';
    showGrid();

    const headerCard = document.createElement('div');
    headerCard.style.gridColumn = '1 / -1';
    headerCard.style.background = 'linear-gradient(135deg, #3b485d, #28303d)';
    headerCard.style.padding = '20px';
    headerCard.style.borderRadius = '8px';
    headerCard.style.display = 'flex';
    headerCard.style.justifyContent = 'space-between';
    headerCard.style.alignItems = 'center';
    headerCard.style.marginBottom = '15px';

    headerCard.innerHTML = `
        <div>
            <h2 style="margin:0 0 5px 0; font-size:18px; color:#fff;">Связка: ${currentPreset.name}</h2>
            <p style="margin:0; font-size:13px; color:var(--text-muted);">Пакетный запуск вложенных скриптов и лаб</p>
        </div>
        <button id="btn-run-entire-preset" style="background:#22c55e; border:none; color:#fff; font-weight:700; padding:12px 24px; border-radius:6px; cursor:pointer; font-size:14px; transition: 0.2s;">
            🚀 ЗАПУСТИТЬ СВЯЗКУ
        </button>
    `;
    gridContainerEl.appendChild(headerCard);

    document.getElementById('btn-run-entire-preset').addEventListener('click', async () => {
        try {
            await apiService.launchPreset(presetId);
        } catch (err) {
            alert('Ошибка пакетного запуска.');
        }
    });

    const rIds = currentPreset.resource_ids || currentPreset.resourceIds || currentPreset.resources || []; 
    const filteredResources = appState.resources.filter(r => rIds.includes(r.id));

    if (filteredResources.length === 0) {
        const placeholder = document.createElement('div');
        placeholder.style.gridColumn = '1 / -1';
        placeholder.style.textAlign = 'center';
        placeholder.style.padding = '40px';
        placeholder.style.color = 'var(--text-muted)';
        placeholder.textContent = 'В этой связке еще нет ресурсов. Привяжи их в меню настроек любого приложения.';
        gridContainerEl.appendChild(placeholder);
        return;
    }

    filteredResources.forEach(res => {
        const card = document.createElement('div');
        card.classList.add('card');
        
        const imgSrc = res.avatar_url && res.avatar_url.startsWith('/home/') 
            ? `${API_BASE_URL}/host-media?path=${encodeURIComponent(res.avatar_url)}`
            : res.avatar_url;

        if (res.avatar_url) {
            card.innerHTML = `
                <div class="card-preview" style="background: url('${imgSrc}') center/cover no-repeat; border-radius: 8px;"></div>
                <div class="card-title">${res.name}</div>
            `;
        } else {
            card.innerHTML = `
                <div class="card-preview"></div>
                <div class="card-title">${res.name}</div>
            `;
        }
        
        card.addEventListener('click', () => showResourceDetails(res));
        gridContainerEl.appendChild(card);
    });
}

function showResourceDetails(resource) {
    appState.selectedResource = resource;
    appState.temporaryBase64Image = resource.avatar_url || "";
    
    if (inputDetailName) inputDetailName.value = resource.name;
    if (inputDetailPath) inputDetailPath.value = resource.path;
    if (inputDetailDescription) inputDetailDescription.value = resource.description || ''; 
    
    if (resource.avatar_url) {
        const imgSrc = resource.avatar_url.startsWith('/home/') 
            ? `${API_BASE_URL}/host-media?path=${encodeURIComponent(resource.avatar_url)}`
            : resource.avatar_url;
            
        detailImgRender.src = imgSrc;
        detailImgRender.style.display = 'block';
        detailPlaceholderSvg.style.display = 'none';
    } else {
        detailImgRender.style.display = 'none';
        detailPlaceholderSvg.style.display = 'block';
    }

    let presetManagerZone = document.getElementById('detail-preset-manager');
    if (!presetManagerZone) {
        presetManagerZone = document.createElement('div');
        presetManagerZone.id = 'detail-preset-manager';
        presetManagerZone.style.margin = '20px 0';
        presetManagerZone.style.padding = '20px';
        presetManagerZone.style.background = '#1a222d'; 
        presetManagerZone.style.border = '2px solid #3b485d';
        presetManagerZone.style.borderRadius = '10px';
        presetManagerZone.style.boxShadow = '0 4px 12px rgba(0,0,0,0.2)';
        
        if (inputDetailDescription) {
            detailArea.insertBefore(presetManagerZone, inputDetailDescription);
        } else {
            detailArea.insertBefore(presetManagerZone, document.querySelector('.detail-actions') || detailArea.lastChild);
        }
    }

    presetManagerZone.innerHTML = `
        <h3 style="font-size:13px; color:#fff; margin:0 0 15px 0; font-weight:700; text-transform: uppercase; letter-spacing: 0.8px; display: flex; align-items: center; gap: 6px;">
            <span>⚡</span> Включение ресурса в связки автозапуска:
        </h3>
        <div id="preset-badges-container" style="display: flex; flex-wrap: wrap; gap: 10px;"></div>
    `;
    
    const badgesContainer = document.getElementById('preset-badges-container');

    appState.presets.forEach(preset => {
        if (!preset.resource_ids) preset.resource_ids = [];
        const isIncluded = preset.resource_ids.includes(resource.id);

        const badge = document.createElement('div');
        badge.style.padding = '10px 18px';
        badge.style.borderRadius = '8px';
        badge.style.fontSize = '14px';
        badge.style.fontWeight = '700';
        badge.style.cursor = 'pointer';
        badge.style.transition = 'all 0.15s ease-in-out';
        badge.style.userSelect = 'none';
        badge.style.display = 'flex';
        badge.style.alignItems = 'center';
        badge.style.gap = '8px';

        if (isIncluded) {
            badge.style.background = '#22c55e';
            badge.style.color = '#fff';
            badge.style.border = '2px solid #22c55e';
            badge.innerHTML = `<span style="font-size: 16px;">✓</span> <span>${preset.name}</span>`;
        } else {
            badge.style.background = '#2d3545';
            badge.style.color = '#9aa5b5';
            badge.style.border = '2px solid #45536a';
            badge.innerHTML = `<span style="font-size: 16px;">+</span> <span>${preset.name}</span>`;
        }

        badge.addEventListener('click', async () => {
            try {
                if (isIncluded) {
                    preset.resource_ids = preset.resource_ids.filter(id => id !== resource.id);
                    await apiService.removeResourceFromPreset(preset.id, resource.id);
                } else {
                    preset.resource_ids.push(resource.id);
                    await apiService.addResourceToPreset(preset.id, resource.id);
                }

                showResourceDetails(resource);
            } catch (err) {
                alert('Не удалось изменить состав связки.');
            }
        });

        badge.addEventListener('mouseenter', () => {
            badge.style.transform = 'scale(1.03)';
            if (!isIncluded) badge.style.borderColor = '#22c55e';
        });
        badge.addEventListener('mouseleave', () => {
            badge.style.transform = 'scale(1)';
            if (!isIncluded) badge.style.borderColor = '#45536a';
        });

        badgesContainer.appendChild(badge);
    });

    if (detailArea) detailArea.style.display = 'block';
    if (gridContainerEl) gridContainerEl.style.display = 'none';
}

function closeDetailArea() {
    if (detailArea) detailArea.style.display = 'none';
    appState.selectedResource = null;
    appState.temporaryBase64Image = "";
    
    const zone = document.getElementById('detail-preset-manager');
    if (zone) zone.innerHTML = ''; 

    if (appState.activeCategoryId && appState.resources.filter(r => r.category_id === appState.activeCategoryId).length > 0) {
        if (gridContainerEl) gridContainerEl.style.display = 'grid';
    } else if (appState.activePresetId) {
        if (gridContainerEl) gridContainerEl.style.display = 'grid';
    }
}

if (btnChangeImage) {
    btnChangeImage.addEventListener('click', async (e) => {
        e.preventDefault();
        
        try {
            const response = await fetch(`${API_BASE_URL}/resource/select-image`, { method: 'POST' });
            if (!response.ok) throw new Error('Выбор изображения отменен');
            
            const data = await response.json();
            const realHostPath = data.path;
            
            console.log("🔥 Нативный путь картинки с хоста:", realHostPath);
            
            appState.temporaryBase64Image = realHostPath;
            detailImgRender.src = `${API_BASE_URL}/host-media?path=${encodeURIComponent(realHostPath)}`;
            detailImgRender.style.display = 'block';
            detailPlaceholderSvg.style.display = 'none';
            
        } catch (err) {
            console.warn(err.message);
        }
    });
}

if (btnDetailRun) {
    btnDetailRun.addEventListener('click', async () => {
        if (!appState.selectedResource) return;
        try {
            await apiService.launchResource(appState.selectedResource.id);
        } catch (err) {
            alert('Launch failed');
        }
    });
}

if (btnDetailDelete) {
    btnDetailDelete.addEventListener('click', async () => {
        if (!appState.selectedResource) return;
        if (!confirm(`Delete ${appState.selectedResource.name}?`)) return;
        try {
            await apiService.deleteResource(appState.selectedResource.id);
            closeDetailArea();
            await loadDashboard();
        } catch (err) {
            alert('Delete failed');
        }
    });
}

if (btnDetailSave) {
    btnDetailSave.addEventListener('click', async () => {
        if (!appState.selectedResource) return;
        const name = inputDetailName.value.trim();
        const path = inputDetailPath.value.trim();
        const description = inputDetailDescription.value.trim(); 
        if (!name || !path) return;

        try {
            await apiService.updateResource(
                appState.selectedResource.id,
                name,
                path,
                description,
                appState.temporaryBase64Image,
                appState.selectedResource.category_id,
                appState.currentUser.id
            );
            closeDetailArea();
            await loadDashboard();
        } catch (err) {
            alert('Update failed');
        }
    });
}

async function handleAddResourceAction() {
    if (!appState.currentUser || !appState.activeCategoryId) return alert('Select category first!');
    
    const name = prompt('Resource Name:');
    if (!name) return;
    const path = prompt('Path or URL:');
    if (!path) return;

    try {
        await apiService.createResource(name, path, appState.activeCategoryId, appState.currentUser.id);
        await loadDashboard();
    } catch (error) {
        alert('Failed to add resource');
    }
}

const btnAddCollection = document.getElementById('btn-add-collection');
if (btnAddCollection) {
    btnAddCollection.addEventListener('click', async () => {
        if (!appState.currentUser) return alert('No user authorized');
        const name = prompt('New category name:');
        if (!name) return;

        try {
            await apiService.createCategory(name, appState.currentUser.id);
            await loadDashboard();
        } catch (error) {
            alert('Failed to create category');
        }
    });
}

const btnDownload = document.getElementById('btn-download');
if (btnDownload) btnDownload.addEventListener('click', handleAddResourceAction);

if (contentBodyEl) {
    contentBodyEl.addEventListener('click', (e) => {
        if (e.target && e.target.id === 'btn-empty-add-resource') {
            handleAddResourceAction();
        }
    });

    contentBodyEl.addEventListener('dragover', (e) => e.preventDefault());
    contentBodyEl.addEventListener('drop', async (e) => {
        e.preventDefault();
        if (!appState.currentUser || !appState.activeCategoryId) return;
        
        const files = e.dataTransfer.files;
        if (files.length > 0) {
            const file = files[0];
            const name = file.name.split('.').slice(0, -1).join('.') || file.name;
            const path = file.path || file.name;
            
            if (confirm(`Add file "${name}" to collection?`)) {
                try {
                    await apiService.createResource(name, path, appState.activeCategoryId, appState.currentUser.id);
                    await loadDashboard();
                } catch (error) {
                    alert('Drop failed');
                }
            }
        }
    });
}
