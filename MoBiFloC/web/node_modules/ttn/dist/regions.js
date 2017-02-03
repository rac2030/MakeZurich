'use strict';

var regions = ['eu', 'asia-se', 'brazil', 'us-west'].reduce(function (acc, region) {
  acc[region] = region.concat('.thethings.network');
  return acc;
}, {});

var ttnSuffix = /\.thethings\.network$/;

var validate = function validate(region) {
  var reg = region;

  // get the region from regions if it exists
  if (region in regions) {
    reg = regions[region];
  }

  // validate the things network regions
  if (ttnSuffix.test(reg) && !(reg.replace(ttnSuffix, '') in regions)) {
    throw new Error('Invalid The Things Network region \'' + region + '\'');
  }

  return reg;
};

module.exports = {
  validate: validate,
  regions: regions
};