var formats = [
      { 
        "label": "Watch Default",
        "value": "%x" 
      }, { 
        "label": "31.01.2016",
        "value": "%d.%m.%Y" 
      }, { 
        "label": "31.01.16",
        "value": "%d.%m.%y" 
      }, { 
        "label": "01/31/2016",
        "value": "%m/%d/%Y" 
      }, { 
        "label": "01/31/16",
        "value": "%m/%d/%y" 
      }, { 
        "label": "Sunday 31 Jan 2016",
        "value": "%A %d %b %Y" 
      }, { 
        "label": "Sunday 31 Jan 16",
        "value": "%A %d %b %y" 
      }, { 
        "label": "Sun 31 Jan 16",
        "value": "%a %d %b %y" 
      }, { 
        "label": "31 Jan 16",
        "value": "%d %b %y" 
      }, { 
        "label": "31 Jan",
        "value": "%d %b" 
      }, { 
        "label": "31 January",
        "value": "%d %B" 
      }, { 
        "label": "January 31",
        "value": "%B %d" 
      }, { 
        "label": "Sun 31 Jan",
        "value": "%a %d %b" 
      }, { 
        "label": "Sunday",
        "value": "%A" 
      }
    ];

module.exports = [
  {
    "type": "heading",
    "defaultValue": "Display Configuration"
  },
  {
    "type": "select",
    "messageKey": "APP_FMT",
    "defaultValue": "%x",
    "label": "App Format",
    "options": formats
  },
  {
    "type": "select",
    "messageKey": "APP_GLANCE_FMT",
    "defaultValue": "%x",
    "label": "App Glance Format",
    "options": formats
  }, 
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];
