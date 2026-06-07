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
    }
};

let appState = {
    currentUser: null,       
    categories: [],          
    resources: [],
    activeCategoryId: null,
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

    if (detailArea) detailArea.style.display = 'block';
    if (gridContainerEl) gridContainerEl.style.display = 'none';
}

function closeDetailArea() {
    if (detailArea) detailArea.style.display = 'none';
    appState.selectedResource = null;
    appState.temporaryBase64Image = "";
    if (appState.activeCategoryId && appState.resources.filter(r => r.category_id === appState.activeCategoryId).length > 0) {
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
