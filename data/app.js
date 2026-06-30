const API = '';

async function fetchAPI(path, method = 'GET', body = null) {
    const opts = { method, headers: {} };
    if (body) { opts.headers['Content-Type'] = 'application/json'; opts.body = JSON.stringify(body); }
    const r = await fetch(API + path, opts);
    return r.json();
}

async function refresh() {
    try {
        const s = await fetchAPI('/api/status');
        document.getElementById('pet-name').textContent = s.name || 'Dragon';
        document.getElementById('stage-badge').textContent = ['Egg','Baby','Teen','Adult','Final'][s.stage] || '?';
        document.getElementById('mood').textContent = s.state || '?';
        document.getElementById('points').textContent = s.points || 0;
        if (s.bagCounts) {
            for (let i = 0; i < 5; i++) {
                const el = document.getElementById('food' + i);
                if (el) el.textContent = s.bagCounts[i];
            }
        }

        const h = await fetchAPI('/api/history');
        const el = document.getElementById('history');
        if (Array.isArray(h) && h.length) {
            el.innerHTML = h.map(e =>
                `<div class="history-entry">⭐+${e.points} ${e.reason||''} ${new Date(e.ts).toLocaleTimeString()}</div>`
            ).join('');
        } else {
            el.innerHTML = '<div class="history-entry">No rewards yet</div>';
        }
    } catch(e) { console.error(e); }
}

async function sendReward(pts) {
    const pin = document.getElementById('pin').value || '0000';
    const reason = document.getElementById('reason').value || '';
    const r = await fetchAPI('/api/reward', 'POST', { points: pts, pin, reason });
    toast(r.ok ? `⭐ +${pts}!` : 'Wrong PIN');
    refresh();
}

async function sendCustomReward() {
    const pts = parseInt(document.getElementById('custom-pts').value) || 3;
    await sendReward(pts);
}

async function showPinChange() {
    const oldPin = prompt('Current PIN:');
    if (!oldPin) return;
    const newPin = prompt('New PIN (4 digits):');
    if (!newPin || newPin.length !== 4) return;
    const r = await fetchAPI('/api/pin', 'POST', { oldPin, newPin });
    toast(r.ok ? 'PIN updated' : 'Wrong old PIN');
}

function toast(msg) {
    const old = document.querySelector('.toast');
    if (old) old.remove();
    const el = document.createElement('div');
    el.className = 'toast';
    el.textContent = msg;
    document.body.appendChild(el);
    setTimeout(() => el.remove(), 2000);
}

setInterval(refresh, 5000);
refresh();
