import { defineStore } from 'pinia'
import { IsCourseArrive, IsEventArrive } from '@/api/event'
import { ref } from 'vue'

export const usePollingStore = defineStore('polling', () => {
    const pollingCourse = ref({});
    const pollingEvent = ref({});
    const is_arrive = ref(0);
    const GetArrivedCourse = async () => {
        try {
            const res = await IsCourseArrive();

            if (res.data.status_code == 0) {
                pollingCourse.value = res.data.course_info;
                is_arrive.value = res.data.is_arrive;
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
                pollingEvent.value = res.data.activity_info;
                is_arrive.value = res.data.is_arrive;
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
        is_arrive,
        GetArrivedCourse,
        GetArrivedEvent
    }
})
