import { defineStore } from "pinia";
export const useTimeStore = defineStore('time',{
    state: () => {
        return {
            Tm: 1,
            week: 1,
            day:1,
            hour: 0,
            minute: 0,
            second:0,
        }
    },
});

