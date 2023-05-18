import { defineStore } from 'pinia'
import { IsCourseArrive, IsEventArrive } from '@/api/event'
import { ref } from 'vue'

export const usePollingStore = defineStore('polling', () => {
    const pollingCourse = ref([]);
    const pollingEvent = ref([]);
    const GetArrivedCourse = async () => {
        try {
            const res = await IsCourseArrive();

            if (res.data.status_code == 0) {
                pollingCourse.value = res.data.pollingCourse
                return true
            }
            return false
        } catch (err) {
            return false
        }
    }
    const GetArrivedEvent = async () => {
        try {
            const res = await IsEventArrive();

            if (res.data.status_code == 0) {
                pollingEvent.value = res.data.pollingEvent
                return true
            }
            return false
        } catch (err) {
            return false
        }
    }
    return {
        pollingCourse,
        pollingEvent,
        GetArrivedCourse,
        GetArrivedEvent
    }
})
