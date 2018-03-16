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
        "label": "2016-01-31",
        "value": "%F" 
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
var time_formats = [
      { 
        "label": "No time",
        "value": "" 
      }, { 
        "label": "14:55",
        "value": "%R" 
      }, { 
        "label": "14:55:02",
        "value": "%T" 
     }, { 
        "label": "02:55 PM",
        "value": "%I:%M %P" 
      }, { 
        "label": "02:55:02 pm",
        "value": "%r" 
     }
    ];

module.exports = [
  {
    "type": "heading",
    "defaultValue": "Data Format"
  },
  {
    "type": "select",
    "messageKey": "APP_FMT",
    "defaultValue": "%x",
    "label": "Application date:",
    "options": formats
  },
  {
    "type": "select",
    "messageKey": "APP_TIME_FMT",
    "defaultValue": "%x",
    "label": "App time:",
    "options": time_formats
  }, 
  {
    "type": "select",
    "messageKey": "APP_GLANCE_FMT",
    "defaultValue": "%x",
    "label": "App Glance date:",
    "options": formats
  }, 
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];
