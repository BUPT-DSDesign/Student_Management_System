<template>
  <div>
    <h2 class="weektitle">第 {{ mytime.week }} 周 — 星期 {{ mytime.day }} — {{ hourmap() }}:{{ minutemap() }}</h2>
      <el-slider v-model="mytime.Tm" show-input input-size="mini" :max=100>
      </el-slider>
  </div>
</template>
<script>
import { TimeStore } from "@/store/time"

const map = {
  0: '00',
  1: '01',
  2: '02',
  3: '03',
  4: '04',
  5: '05',
  6: '06',
  7: '07',
  8: '08',
  9: '09',
}
var timer1 = setInterval(function () {
  TimeStore.minute+= TimeStore.Tm;  
    if (TimeStore.minute >= 60) {
      TimeStore.minute %= 60;
      TimeStore.hour += 1;
      if (TimeStore.hour >= 24) {
        TimeStore.hour %= 24;
        TimeStore.day += 1;
        if (TimeStore.day >= 7) {
          TimeStore.day %= 7;
          TimeStore.week += 1;
        }
      }
    }
}, 1000);

export default {
  data() {
    return {
      mytime: TimeStore
    }
  },
  methods: {

  },
  computed: {
    hourmap() {
      return function () {
        if (TimeStore.hour < 10) {
          return map[TimeStore.hour];
        }
        else
          return TimeStore.hour;
      }
    },
    minutemap() {
      return function () {
        if (TimeStore.minute < 10) {
          return map[TimeStore.minute];
        }
        else
          return TimeStore.minute;
      }
    },
  }
}

</script>
<style>
@font-face {
  font-family: mFont;
  src: url(@/assets/font/LcdD.ttf);
}

.weektitle {
  margin-left: 20px;
  margin-top: 10px;
  color: #eee;
  float: left;
  font-family: mFont;
  width: 300px;
}
.el-slider{
  margin-top:10px;
  width:200px;
  float: left;
}
</style>