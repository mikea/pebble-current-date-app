var formats = [
      { 
        "label": "Watch Default",
        "value": "%x" 
      }, { 
        "label": "Thursday 01 Jan 15",
        "value": "%A %d %b %y" 
      }, { 
        "label": "Thu 01 Jan 15",
        "value": "%a %d %b %y" 
      }, { 
        "label": "01 Jan 15",
        "value": "%d %b %y" 
      }, { 
        "label": "01 Jan",
        "value": "%d %b" 
      }, { 
        "label": "01 January",
        "value": "%d %B" 
      }, { 
        "label": "January 01",
        "value": "%B %d" 
      }, { 
        "label": "Thu 01 Jan",
        "value": "%a %d %b" 
      }, { 
        "label": "Thursday",
        "value": "%A" 
      }
    ];

module.exports = [
  {
    "type": "heading",
    "defaultValue": "App Configuration"
  },
  {
    "type": "select",
    "messageKey": "APP_FMT",
    "defaultValue": "%x",
    "label": "Application Format",
    "options": formats
  },
  {
    "type": "select",
    "messageKey": "APP_GLANCE_FMT",
    "defaultValue": "%x",
    "label": "Glance Format",
    "options": formats
  }, 
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];
