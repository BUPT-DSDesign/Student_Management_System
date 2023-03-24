<template>
  <div>
    <h2 class="weektitle">第 {{ mytime.week }} 周 — 星期 {{ mytime.day }} — {{ hourmap() }}:{{ minutemap() }}:{{ secondmap()
    }}</h2>
    <!-- <el-input v-model="input" placeholder="请输入内容"  style="width:70px"></el-input> -->
      <el-slider v-model="mytime.Tm" show-input input-size="mini" max=1000>
      </el-slider>
  </div>
</template>
<script>
import { useTimeStore } from "@/store/time"


const timestore = useTimeStore();
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
  console.log(timestore.Tm)
  timestore.second += timestore.Tm;
  if (timestore.second >= 60) {
    timestore.second %= 60;
    timestore.minute += 1;
    if (timestore.minute == 60) {
      timestore.minute %= 60;
      timestore.hour += 1;
      if (timestore.hour == 24) {
        timestore.hour %= 24;
        timestore.day += 1;
        if (timestore.day == 7) {
          timestore.day %= 7;
          timestore.week += 1;
        }
      }
    }
  }
}, 1000);

export default {
  data() {
    return {
      mytime: timestore,
    }
  },
  methods: {

  },
  computed: {
    hourmap() {
      return function () {
        if (this.mytime.hour < 10) {
          return map[this.mytime.hour];
        }
        else
          return this.mytime.hour;
      }
    },
    minutemap() {
      return function () {
        if (this.mytime.minute < 10) {
          return map[this.mytime.minute];
        }
        else
          return this.mytime.minute;
      }
    },
    secondmap() {
      return function () {
        if (this.mytime.second < 10) {
          return map[this.mytime.second];
        }
        else
          return this.mytime.second;
      }
    }
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