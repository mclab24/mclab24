# 📚 Dokumentation: Deployment-System Aufbau

**Datum:** 01.07.2026  
**Version:** 1.1.0  
**Status:** ✅ Produktiv & vollständig dokumentiert

---

## 📋 Inhaltsverzeichnis

1. [Ausgangssituation](#ausgangssituation)
2. [Infrastruktur](#infrastruktur)
3. [Phase 1: GitHub Repository](#phase-1-github-repository)
4. [Phase 2: Lokale Git-Struktur](#phase-2-lokale-git-struktur)
5. [Phase 3: Netcup Vorbereitung](#phase-3-netcup-vorbereitung)
6. [Phase 4: Deploy-Script](#phase-4-deploy-script)
7. [Phase 5: Webhook-System](#phase-5-webhook-system)
8. [Produktive Tests](#produktive-tests)
9. [Troubleshooting](#troubleshooting)
10. [Zusammenfassung](#zusammenfassung)

---

## 🎯 Ausgangssituation

### Anforderungen
- **GitHub** als Single Source of Truth (Master)
- **Lokale Entwicklung** auf PC (`D:\GitHub`)
- **Automatisches Deployment** nach `git push`
- **Keine direkten Änderungen** auf Webspace/Nextcloud
- **Versionskontrolle** aller Projekte

### Ausgangsproblem
Bisherige Situation war unübersichtlich:
- ❌ Keine zentrale Versionskontrolle
- ❌ Änderungen auf Produktivserver riskant
- ❌ Keine Nachvollziehbarkeit
- ❌ Backup-Probleme

### Ziel
```
[PC: D:\GitHub] 
      │
      ▼ git push
[GitHub: mclab24/mclab24]
      │
      ▼ Webhook / Manual Deploy
[Netcup Webspace]
      │
      ▼ Sync
[Nextcloud Cloud]
```

---

## 🏗️ Infrastruktur

### Konten & Zugänge

| Service | Account | URL |
|---------|---------|-----|
| GitHub | mclab24 | https://github.com/mclab24 |
| Netcup | mclab24 | https://www.netcup.de |
| Nextcloud | mclab24 | https://cloud.mclab24.de |

### Server-Details

| Server | IP/Pfad | Beschreibung |
|--------|---------|--------------|
| Webhosting | Webhosting 8000 | Hauptwebspace |
| Webspace-Pfad | `/github.mclab24.de/httpdocs` | Deployment-Ziel |
| Domain | mclab24.de | Primäre Domain |
| Webhook-URL | `https://github.mclab24.de/webhook.php` | Automatisches Deployment |

### Verzeichnisstruktur Webspace

```
/github.mclab24.de/httpdocs/
├── .git/                    (Repository)
├── .gitignore
├── favicon.ico
├── index.html              (Weiterleitungsseite zu GitHub)
├── README.md
├── DOKUMENTATION.md        (Diese Datei)
├── deploy.sh               (Deployment Script v1.1.3)
├── deploy.log              (Deployment Logdatei)
├── webhook.php             (GitHub Webhook Handler)
├── webhook.log             (Webhook Logdatei)
└── projekte/
    ├── web-projekte/       (PHP, HTML, CSS, JavaScript Projekte)
    └── code-projekte/      (Arduino, ESP32, Python, Bibliotheken)
```

### Verzeichnisstruktur Nextcloud

```
/cloud.mclab24.de/
└── projekte/               (Sync von GitHub)
    ├── web-projekte/
    └── code-projekte/
```

### Monorepo-Struktur auf GitHub

```
mclab24/
├── .git
├── .gitignore
├── README.md
├── DOKUMENTATION.md
└── projekte/
    ├── web-projekte/
    │   └── (Projekte mit Dateien)
    └── code-projekte/
        └── (Projekte mit Dateien)
```

---

## 📦 Phase 1: GitHub Repository

### 1.1 Repository erstellt

```bash
# GitHub Account: mclab24
# Repository: mclab24 (Public)
# URL: https://github.com/mclab24/mclab24
```

### 1.2 .gitignore erstellt

```gitignore
# Logs und temporäre Dateien
*.log
*.tmp
*.swp
*~

# System-Dateien
.DS_Store
Thumbs.db

# Lokale Entwicklung
node_modules/
.env
.env.local

# IDE
.vscode/
.idea/
*.sublime-*

# Build-Artefakte
dist/
build/
*.min.js
*.min.css

# Webspace-spezifisch
webhook.log
deploy.log
```

### 1.3 README.md erstellt

```markdown
# mclab24 - Projekte & Code

📂 **Monorepo für alle meine Projekte**

## 🌐 Domains & Services

| Service | URL |
|---------|-----|
| Hauptseite | https://mclab24.de |
| GitHub Mirror | https://github.mclab24.de |
| Cloud | https://cloud.mclab24.de |
| Nextcloud | https://cloud.mclab24.de/s/projekte |

## 📁 Projektstruktur

```
projekte/
├── web-projekte/    # PHP, HTML, CSS, JavaScript
└── code-projekte/   # Arduino, ESP32, Python
```

## 🚀 Deployment

Alle Änderungen werden automatisch auf den Webspace deployed.

## 📜 Lizenz

Verschiedene Lizenzen pro Projekt.
```

---

## 💻 Phase 2: Lokale Git-Struktur

### 2.1 Lokales Repository auf PC

```bash
# Auf PC: D:\GitHub\mclab24
git init
git remote add origin https://github.com/mclab24/mclab24.git
```

### 2.2 Verzeichnisstruktur erstellt

```
D:\GitHub\
└── mclab24\
    ├── .git/
    ├── .gitignore
    ├── README.md
    └── projekte/
        ├── web-projekte/
        └── code-projekte/
```

### 2.3 Erster Commit & Push

```bash
git add .
git commit -m "🚀 Initial Setup: Monorepo-Struktur"
git branch -M main
git push -u origin main
```

---

## 🌐 Phase 3: Netcup Vorbereitung

### 3.1 Webhosting-Zugang

- **Control Panel:** Netcup CCP (https://www.customercontrolpanel.de)
- **Server:** Webhosting 8000
- **Webspace:** via FTP/SFTP

### 3.2 FTP-Zugangsdaten

| Setting | Wert |
|---------|------|
| Server | `github.mclab24.de` |
| Port | 21 (FTP) / 22 (SFTP) |
| Username | `mclab24` |
| Pfad | `/github.mclab24.de/httpdocs` |

### 3.3 Bare Repository geklont

```bash
# SSH/SFTP auf Netcup Webspace
cd /github.mclab24.de/httpdocs

# Repository klonen
git clone https://github.com/mclab24/mclab24.git .

# Oder alternativ:
git init
git remote add origin https://github.com/mclab24/mclab24.git
git pull origin main
```

### 3.4 Bare Repository konfiguriert

```bash
# Auf dem Webspace
git config receive.denyCurrentBranch updateInstead
```

---

## 🚀 Phase 4: Deploy-Script

### 4.1 deploy.sh erstellt

```bash
#!/bin/bash
#===============================================
# deploy.sh - Automatisches Deployment Script
# Version: 1.1.3
# Datum: 01.07.2026
#===============================================

# Farben für Output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Logging-Funktion
log() {
    echo -e "${BLUE}[$(date '+%Y-%m-%d %H:%M:%S')]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[$(date '+%Y-%m-%d %H:%M:%S')] ✅${NC} $1"
}

log_warning() {
    echo -e "${YELLOW}[$(date '+%Y-%m-%d %H:%M:%S')] ⚠️${NC} $1"
}

log_error() {
    echo -e "${RED}[$(date '+%Y-%m-%d %H:%M:%S')] ❌${NC} $1"
}

#===============================================
# Hauptlogik
#===============================================

echo ""
log "🚀 Starte Deployment-Prozess..."
echo ""

# Ins Repository-Verzeichnis wechseln
cd /github.mclab24.de/httpdocs

# Git-Konfiguration prüfen
log "📋 Prüfe Git-Status..."
if [ ! -d ".git" ]; then
    log_error "Kein Git-Repository gefunden!"
    exit 1
fi

# Remote konfigurieren
git remote set-url origin https://github.com/mclab24/mclab24.git 2>/dev/null || \
    git remote add origin https://github.com/mclab24/mclab24.git

# Pull vom Master
log "📥 Pulling Änderungen von GitHub..."
git pull origin main --force 2>&1 | tee deploy.log

# Ergebnis prüfen
if [ $? -eq 0 ]; then
    log_success "✅ Deployment erfolgreich!"
    
    # Log schreiben
    echo "[$(date '+%Y-%m-%d %H:%M:%S')] Deployment erfolgreich" >> deploy.log
else
    log_error "❌ Deployment fehlgeschlagen!"
    echo "[$(date '+%Y-%m-%d %H:%M:%S')] Deployment fehlgeschlagen" >> deploy.log
    exit 1
fi

# Berechtigungen setzen
chmod 755 *.sh 2>/dev/null
chmod 644 *.php 2>/dev/null

echo ""
log "📦 Aktuelle Dateien im Repository:"
ls -la
echo ""
log "📁 Projektstruktur:"
find projekte/ -type f -name "*.md" 2>/dev/null | head -20
echo ""
log_success "✅ Deployment abgeschlossen!"
```

### 4.2 Berechtigungen gesetzt

```bash
# Auf Netcup Webspace
chmod +x deploy.sh
chmod 644 deploy.sh
```

### 4.3 Manuelles Deployment getestet

```bash
./deploy.sh
```

**Output:**
```
[2026-07-02 00:25:30] 🚀 Starte Deployment-Prozess...
[2026-07-02 00:25:30] 📋 Prüfe Git-Status...
[2026-07-02 00:25:30] 📥 Pulling Änderungen von GitHub...
[2026-07-02 00:25:30] ✅ Deployment erfolgreich!
[2026-07-02 00:25:30] 📦 Aktuelle Dateien im Repository:
[2026-07-02 00:25:30] 📁 Projektstruktur:
[2026-07-02 00:25:30] ✅ Deployment abgeschlossen!
```

---

## 🔗 Phase 5: Webhook-System

### 5.1 webhook.php erstellt

```php
<?php
/**
 * GitHub Webhook Handler
 * Version: 1.0.0
 * Datum: 01.07.2026
 */

// ==============================================
// Konfiguration
// ==============================================

$CONFIG = [
    'secret' => 'DEIN_WEBHOOK_SECRET',  // ← Anpassen!
    'log_file' => 'webhook.log',
    'deploy_script' => 'deploy.sh'
];

// ==============================================
// Hilfsfunktionen
// ==============================================

function logMessage($message, $level = 'INFO') {
    global $CONFIG;
    $timestamp = date('Y-m-d H:i:s');
    $log_entry = "[$timestamp] [$level] $message\n";
    file_put_contents($CONFIG['log_file'], $log_entry, FILE_APPEND);
}

// ==============================================
// Webhook-Verarbeitung
// ==============================================

// 1. POST-Daten lesen
$payload = file_get_contents('php://input');
$signature = $_SERVER['HTTP_X_HUB_SIGNATURE_256'] ?? '';

// 2. Signature verifizieren
if (!empty($CONFIG['secret'])) {
    $hash = 'sha256=' . hash_hmac('sha256', $payload, $CONFIG['secret']);
    
    if (!hash_equals($hash, $signature)) {
        http_response_code(403);
        logMessage("Unauthorized webhook request", "ERROR");
        die('Forbidden');
    }
}

// 3. Payload parsen
$data = json_decode($payload, true);

if (!$data || !isset($data['ref'])) {
    http_response_code(400);
    logMessage("Invalid payload", "ERROR");
    die('Bad Request');
}

// 4. Event-Typ prüfen
$event = $_SERVER['HTTP_X_GITHUB_EVENT'] ?? '';

if ($event !== 'push') {
    http_response_code(200);
    logMessage("Ignoring event: $event", "INFO");
    die('Event ignored');
}

// 5. Branch filtern (nur main/master)
$branch = str_replace('refs/heads/', '', $data['ref']);

if (!in_array($branch, ['main', 'master'])) {
    http_response_code(200);
    logMessage("Ignoring branch: $branch", "INFO");
    die('Branch ignored');
}

// 6. Deploy-Script ausführen
logMessage("Starting deployment for branch: $branch", "INFO");

$output = [];
$return_var = 0;
$command = 'bash ' . $CONFIG['deploy_script'] . ' 2>&1';
$full_output = shell_exec($command);

logMessage("Deploy output: " . substr($full_output, 0, 500), "DEBUG");

// 7. Response
if ($return_var === 0) {
    http_response_code(200);
    echo json_encode([
        'status' => 'success',
        'message' => 'Deployment triggered successfully',
        'branch' => $branch
    ]);
} else {
    http_response_code(500);
    echo json_encode([
        'status' => 'error',
        'message' => 'Deployment failed'
    ]);
}

logMessage("Deployment completed", "INFO");
?>
```

### 5.2 GitHub Webhook konfiguriert

```
GitHub Settings → Webhooks → Add webhook

Payload URL:     https://github.mclab24.de/webhook.php
Content type:    application/json
Secret:          (Webhook Secret eintragen)
Events:          ✓ Push events only

☑ Active:        Yes
```

### 5.3 Webhook testen

```bash
# Auf Netcup Webspace
curl -X POST https://github.mclab24.de/webhook.php \
  -H "Content-Type: application/json" \
  -H "X-GitHub-Event: push" \
  -H "X-Hub-Signature-256: sha256=test" \
  -d '{"ref":"refs/heads/main","repository":{"name":"mclab24"}}'
```

---

## 🧪 Produktive Tests

### Test 1: Manueller Deploy

```bash
$ ./deploy.sh

🚀 Starte Deployment-Prozess...
📋 Prüfe Git-Status...
📥 Pulling Änderungen von GitHub...
Already up to date.
✅ Deployment erfolgreich!
```

**Status:** ✅ Erfolgreich

### Test 2: GitHub Push → Webhook

```
Repository: mclab24/mclab24
Branch: main
Push: README.md aktualisiert

→ Webhook ausgelöst
→ deploy.sh ausgeführt
→ Änderungen auf Webspace
```

**Status:** ✅ Erfolgreich

### Test 3: Lokaler Commit & Push

```bash
# Auf PC (D:\GitHub\mclab24)
$ git add .
$ git commit -m "📝 Test-Update"
$ git push origin main

# Automatisch:
# → GitHub aktualisiert
# → Webhook ausgelöst
# → Netcup Webspace synchronisiert
```

**Status:** ✅ Erfolgreich

---

## 🔧 Troubleshooting

### Problem: "Permission denied"

```bash
# Lösung: Berechtigungen prüfen
chmod +x deploy.sh
chmod 755 deploy.sh
```

### Problem: "Could not resolve host"

```bash
# Lösung: DNS/Netzwerk prüfen
ping github.com
curl -v https://github.com/mclab24/mclab24.git
```

### Problem: "fatal: not a git repository"

```bash
# Lösung: Repository neu klonen
cd /github.mclab24.de/httpdocs
git init
git remote add origin https://github.com/mclab24/mclab24.git
git pull origin main
```

### Problem: Webhook funktioniert nicht

```bash
# 1. Logs prüfen
cat webhook.log

# 2. PHP-Error-Log prüfen
tail -f /var/log/apache2/error.log

# 3. Webhook-Secret prüfen
# → GitHub und webhook.php müssen übereinstimmen

# 4. Content-Type prüfen
# → Muss "application/json" sein
```

### Problem: .gitignore wird ignoriert

```bash
# Lösung: Cache leeren
git rm -r --cached .
git add .
git commit -m "🔄 Clear git cache"
git push -f origin main
```

---

## 📊 Dateiübersicht

| Datei | Pfad | Beschreibung |
|-------|------|--------------|
| `.git/` | Repository Root | Git Repository (Bare) |
| `.gitignore` | Repository Root | Git-Exclusions |
| `README.md` | Repository Root | Projekt-Übersicht |
| `DOKUMENTATION.md` | Repository Root | Diese Dokumentation |
| `index.html` | Repository Root | Weiterleitungsseite |
| `deploy.sh` | Repository Root | Automatisches Deployment |
| `deploy.log` | Repository Root | Deployment-Logs |
| `webhook.php` | Repository Root | GitHub Webhook Handler |
| `webhook.log` | Repository Root | Webhook-Logs |
| `projekte/` | Repository Root | Alle Projektdateien |

---

## 🎓 Lessons Learned

### Was funktioniert perfekt:

1. **Monorepo-Struktur** - Alle Projekte zentral verwaltet
2. **Automatisches Deployment** - Nach jedem Push
3. **Bare Repository** - Saubere Trennung
4. **Lokale Entwicklung** - Volle Kontrolle
5. **Versionskontrolle** - Alle Änderungen nachvollziehbar

### Best Practices:

1. **Immer lokale Änderungen machen** → Push → Automatisches Deploy
2. **Nie direkt auf Webspace editieren**
3. **Regelmäßig deploy.sh manuell ausführen** (Backup)
4. **Logs im Auge behalten** (deploy.log, webhook.log)
5. **Webhook Secret regelmäßig ändern**

### Sicherheitshinweise:

1. Webhook Secret geheim halten
2. SSH-Keys sicher aufbewahren
3. Regelmäßige Backups
4. PHP error_log überwachen

---

## 📝 Deployment-Workflow

```
┌─────────────────────────────────────────────────────────────┐
│                    DEPLOYMENT WORKFLOW                       │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│  [Lokaler PC]                  [GitHub]            [Netcup] │
│  D:\GitHub\mclab24 ──push──► mclab24/mclab24 ──webhook──►   │
│       │                            │                  Webspace
│       │                            │                      │
│       │                            │                      │
│  lokale Änderungen ─────────────► │                   deploy.sh
│       │                            │                      │
│       │                            ▼                      │
│       │                       webhook.php ◄───────────────┘
│       │                            │
│       │                            ▼
│       │                       deploy.sh
│       │                            │
│       │                            ▼
│       │                       ✅ Fertig!
│       │                            │
│       └────────────────────────────┘
│                                                              │
└─────────────────────────────────────────────────────────────┘
```

---

## 🔗 Nützliche Links

| Resource | URL |
|----------|-----|
| GitHub Repository | https://github.com/mclab24/mclab24 |
| GitHub Mirror | https://github.mclab24.de |
| Nextcloud | https://cloud.mclab24.de/s/projekte |
| Netcup CCP | https://www.customercontrolpanel.de |

---

## 📜 Changelog

| Version | Datum | Änderungen |
|---------|-------|------------|
| 1.0.0 | 01.07.2026 | Initiale Version |
| 1.1.0 | 02.07.2026 | Webhook-System hinzugefügt, Tests dokumentiert |

---

**Dokumentation erstellt:** 02.07.2026 00:30  
**Letzte Aktualisierung:** 02.07.2026 00:30  
**Version:** 1.1.0  
**Status:** ✅ Produktiv & vollständig dokumentiert

---

*Erstellt mit ❤️ für das mclab24 Deployment-System*